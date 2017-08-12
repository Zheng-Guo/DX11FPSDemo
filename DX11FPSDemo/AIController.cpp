#include <cstdlib>
#include <ctime>
#include "AIController.h"

using namespace std;

void AIController::characterMoveTo(int x, int z)
{
	XMFLOAT3 currentPosition = controlledCharacter->getPosition();
	float xCoord = x * 2 + 1, zCoord = z * 2 + 1;
	float distanceSquared = (currentPosition.x - xCoord)*(currentPosition.x - xCoord) + (currentPosition.z - zCoord)*(currentPosition.z - zCoord);
	if (distanceSquared >= stopRange*stopRange)
	{
		if (currentPath.empty() || x != currentPath[currentPath.size() - 1].first || z != currentPath[currentPath.size() - 1].second)
		{
			int currentX = ((int)currentPosition.x) / 2, currentZ = ((int)currentPosition.z) / 2;
			currentPath = navigationManager->pathFinding(currentX, currentZ, x, z);
		}
		if (currentPath.size() == 0)
			return;
		XMVECTOR nextGridPoint = XMVectorSet(currentPath[0].first * 2 + 1, currentPosition.y, currentPath[0].second * 2 + 1, 1.f);
		if (XMVectorGetX(XMVector3Length(nextGridPoint - XMLoadFloat3(&currentPosition)))<stopRange)
		{
			currentPath.erase(currentPath.begin());
			nextGridPoint = XMVectorSet(currentPath[0].first * 2 + 1, currentPosition.y, currentPath[0].second * 2 + 1, 1.f);
		}
		XMFLOAT3 forwardDirection = controlledCharacter->getForwardDirection();
		XMVECTOR fd = XMLoadFloat3(&forwardDirection);
		XMVECTOR cp = XMLoadFloat3(&currentPosition);
		XMVECTOR newDirection = XMVector3Normalize(nextGridPoint - cp);
		XMVECTOR abv = XMVector3AngleBetweenVectors(fd, newDirection);
		XMVECTOR up = XMVector3Normalize(XMVector3Cross(fd, newDirection));
		XMFLOAT3 upDirection = controlledCharacter->getUpDirection();
		if (XMVector3NearEqual(up, XMLoadFloat3(&upDirection), XMVectorSet(0.001f, 0.001f, 0.001f, 0.f)))
		{
			controlledCharacter->rotate(XMVectorGetX(abv));
		}
		else
		{
			controlledCharacter->rotate(-XMVectorGetX(abv));
		}
		controlledCharacter->moveForward();
	}
}

AIController::AIController() :controlledCharacter(nullptr),
active(true),
isMoving(false),
isPatrolling(false),
isWaiting(false),
stopRange(0.5f),
waitTime(0.f),
waitedTime(0.f),
patrolPointIndex(0),
destination(-1, -1)
{
	navigationManager = NavigationManager::getInstance();
	srand(time(nullptr));
}

void AIController::update(float deltaTime)
{
	XMFLOAT3 enemyPosition = controlledCharacter->getPlayerCharacter()->getPosition();
	XMFLOAT3 currentPosition = controlledCharacter->getPosition();
	float horizontalDistanceSquared = (enemyPosition.x - currentPosition.x)*(enemyPosition.x - currentPosition.x) + (enemyPosition.z - currentPosition.z)*(enemyPosition.z - currentPosition.z);
	if (horizontalDistanceSquared<(controlledCharacter->getGuardRange()*controlledCharacter->getGuardRange()))
	{
		if (horizontalDistanceSquared >= (controlledCharacter->getFireRange()*controlledCharacter->getFireRange()))
		{
			int destX = ((int)enemyPosition.x) / 2, destZ = ((int)enemyPosition.z) / 2;
			characterMoveTo(destX, destZ);
			controlledCharacter->stopAttacking();
		}
		else
		{
			XMFLOAT3 forwardDirection = controlledCharacter->getForwardDirection();
			XMVECTOR fd = XMLoadFloat3(&forwardDirection);
			XMVECTOR newDirection = XMVector3Normalize(XMVectorSet(enemyPosition.x, 0.f, enemyPosition.z, 1.f) - XMVectorSet(currentPosition.x, 0.f, currentPosition.z, 1.f));
			XMVECTOR abv = XMVector3AngleBetweenVectors(fd, newDirection);
			XMVECTOR up = XMVector3Normalize(XMVector3Cross(fd, newDirection));
			XMFLOAT3 upDirection = controlledCharacter->getUpDirection();
			if (XMVector3NearEqual(up, XMLoadFloat3(&upDirection), XMVectorSet(0.001f, 0.001f, 0.001f, 0.f)))
			{
				controlledCharacter->rotate(XMVectorGetX(abv));
			}
			else
			{
				controlledCharacter->rotate(-XMVectorGetX(abv));
			}
			controlledCharacter->attack();
		}
	}
	else if (isPatrolling)
	{
		destination = patrolTargets[patrolPointIndex];
		float destX = destination.first * 2 + 1, destZ = destination.second * 2 + 1;
		float distanceToDestinationSquared = (currentPosition.x - destX)*(currentPosition.x - destX) + (currentPosition.z - destZ)*(currentPosition.z - destZ);
		if (distanceToDestinationSquared >= stopRange*stopRange)
		{
			characterMoveTo(destination.first, destination.second);
		}
		else if (!isWaiting)
		{
			isWaiting = true;
			waitedTime = waitTime;
		}
		else if (waitedTime>0)
		{
			waitedTime -= deltaTime;
		}
		else
		{
			isWaiting = false;
			++patrolPointIndex;
			if (patrolPointIndex >= patrolTargets.size())
				patrolPointIndex = 0;
		}
	}
	else
	{
		float destX, destZ;
		if (destination.first == -1 && destination.second == -1)
		{
			vector<pair<int, int>> availableDestination = navigationManager->getNavigableGridList();
			float wonderRange = controlledCharacter->getWonderRange();
			do
			{
				int i = rand() % availableDestination.size();
				destX = availableDestination[i].first * 2 + 1;
				destZ = availableDestination[i].second * 2 + 1;
				float distanceSquared = (destX - currentPosition.x)*(destX - currentPosition.x) + (destZ - currentPosition.z)*(destZ - currentPosition.z);
				if (distanceSquared >= wonderRange*wonderRange)
				{
					destination = availableDestination[i];
					break;
				}
			} while (true);
		}
		destX = destination.first * 2 + 1;
		destZ = destination.second * 2 + 1;
		float distanceToDestinationSquared = (currentPosition.x - destX)*(currentPosition.x - destX) + (currentPosition.z - destZ)*(currentPosition.z - destZ);
		if (distanceToDestinationSquared >= stopRange*stopRange)
		{
			characterMoveTo(destination.first, destination.second);
		}
		else if (!isWaiting)
		{
			isWaiting = true;
			waitedTime = waitTime;
		}
		else if (waitedTime>0)
		{
			waitedTime -= deltaTime;
		}
		else
		{
			isWaiting = false;
			vector<pair<int, int>> availableDestination = navigationManager->getNavigableGridList();
			float wonderRange = controlledCharacter->getWonderRange();
			do
			{
				int i = rand() % availableDestination.size();
				destX = availableDestination[i].first * 2 + 1;
				destZ = availableDestination[i].second * 2 + 1;
				float distanceSquared = (destX - currentPosition.x)*(destX - currentPosition.x) + (destZ - currentPosition.z)*(destZ - currentPosition.z);
				if (distanceSquared >= wonderRange*wonderRange)
				{
					destination = availableDestination[i];
					break;
				}
			} while (true);
		}

	}
}
