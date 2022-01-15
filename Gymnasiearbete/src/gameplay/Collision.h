#pragma once

#include "../Common.h"

#include "Sprite.h"
#include "Body.h"
#include "BlockGroup.h"

namespace Collision
{
	static void CircleToCircle(Body* bodyA, Body* bodyB, float deltaTime)
	{
		glm::vec2 posA(bodyA->m_PotentialPosition.x, bodyA->m_PotentialPosition.z);
		glm::vec2 posB(bodyB->m_PotentialPosition.x, bodyB->m_PotentialPosition.z);

		float diameter = bodyA->m_ColliderRadius + bodyB->m_ColliderRadius;
		float distance = glm::distance(posA, posB);

		float dif = distance - diameter;

		if (dif <= 0.0f)
		{
			glm::vec2 normal = glm::normalize(posB - posA);

			bodyA->m_PotentialPosition += glm::vec3(normal.x, 0.0f, normal.y) * dif / 2.0f;
			bodyB->m_PotentialPosition -= glm::vec3(normal.x, 0.0f, normal.y) * dif / 2.0f;
		}
	}

	static void CircleToBlocks(Body* body, BlockGroup* blockGroup, float deltaTime)
	{
		glm::vec2 bodyPos(body->m_PotentialPosition.x, body->m_PotentialPosition.z);
		glm::vec2 BlocksPos(blockGroup->m_Position.x, blockGroup->m_Position.z);

		float BlocksRot = blockGroup->m_Rotation.y;

		glm::vec2 localPos = bodyPos - BlocksPos;
		localPos = glm::vec2(
			localPos.x * glm::cos(BlocksRot) - localPos.y * glm::sin(BlocksRot), 
			localPos.x * glm::sin(BlocksRot) + localPos.y * glm::cos(BlocksRot)
		);

		for (int z = round(localPos.y) - 2; z < round(localPos.y) + 2; z++)
		{
			for (int x = round(localPos.x) - 2; x < round(localPos.x) + 2; x++)
			{
				if (blockGroup->GetBlock(glm::vec3(x, 0, z)) != EMPTY)
				{
					glm::vec2 nearestPoint;
					nearestPoint.x = std::max(float(x - 0.5f), std::min(float(x + 0.5f), localPos.x));
					nearestPoint.y = std::max(float(z - 0.5f), std::min(float(z + 0.5f), localPos.y));

					glm::vec2 rayToNearest = nearestPoint - localPos;
					float overlap = body->m_ColliderRadius - glm::length(rayToNearest);

					if (std::isnan(overlap)) overlap = 0.0f;

					if (overlap > 0.0f)
					{
						if (blockGroup->GetBlock(glm::vec3(x, 1, z)) != EMPTY)
						{
							localPos -= glm::normalize(rayToNearest) * overlap;
							body->OnCollision(blockGroup, BlockCollisions::Wall);
							blockGroup->OnCollision(body, BlockCollisions::Wall);
						}
						else
						{
							if (body->m_Position.y >= 0.0f) 
							{
								if (body->m_PotentialPosition.y < 0.0f)
								{
									body->m_PotentialPosition.y = 0.0f;
									body->OnCollision(blockGroup, BlockCollisions::Floor);
									blockGroup->OnCollision(body, BlockCollisions::Floor);
								}
							}
							else
							{
								localPos -= glm::normalize(rayToNearest) * overlap;
								body->OnCollision(blockGroup, BlockCollisions::Side);
								blockGroup->OnCollision(body, BlockCollisions::Side);
							}
						}
					}
				}
			}
		}

		localPos = glm::vec2(
			localPos.x * glm::cos(-BlocksRot) - localPos.y * glm::sin(-BlocksRot),
			localPos.x * glm::sin(-BlocksRot) + localPos.y * glm::cos(-BlocksRot)
		);
		localPos += BlocksPos;

		body->m_PotentialPosition = glm::vec3(localPos.x, body->m_PotentialPosition.y, localPos.y);
	}

	static float CrossProduct(const glm::vec2& a, const glm::vec2& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	static glm::vec2 CrossProduct(const glm::vec2& a, float s)
	{
		return glm::vec2(s * a.y, -s * a.x);
	}

	static glm::vec2 CrossProduct(float s, const glm::vec2& a)
	{
		return glm::vec2(-s * a.y, s * a.x);
	}

	static void BlocksToBlocks(BlockGroup* blockGroupA, BlockGroup* blockGroupB, float deltaTime)
	{
		BlockGroup* bgA = blockGroupA;
		BlockGroup* bgB = blockGroupB;

		if (bgA->m_Static && bgB->m_Static) return;

		// First test A against B then switch pointers
		for (int i = 0; i < 2; i++)
		{
			if (i == 1)
			{
				// Switch pointers
				bgA = blockGroupB;
				bgB = blockGroupA;
			}

			glm::vec2 posA = bgA->m_PotentialPosition;
			glm::vec2 posB = bgB->m_PotentialPosition;

			float rotA = bgA->m_PotentialRotation;
			float rotB = bgB->m_PotentialRotation;

			for (int z = -32; z < 32; z++)
			{
				for (int x = -32; x < 32; x++)
				{
					// Also test offsets to find all corners
					if (bgA->GetBlock(glm::ivec3(x, 0, z)) != EMPTY || 
						bgA->GetBlock(glm::ivec3(x - 1, 0, z)) != EMPTY ||
						bgA->GetBlock(glm::ivec3(x, 0, z - 1)) != EMPTY ||
						bgA->GetBlock(glm::ivec3(x - 1, 0, z - 1)) != EMPTY)
					{
						// A corner of the blockgroup
						glm::vec2 point(x - 0.5f, z - 0.5f);

						// Rotate from A space
						point = glm::vec2(
							point.x * glm::cos(-rotA) - point.y * glm::sin(-rotA),
							point.x * glm::sin(-rotA) + point.y * glm::cos(-rotA)
						);

						// Save point position relative to A position
						glm::vec2 contactPointA = -point;

						// Translate from A space
						point += posA;

						// Translate to B space
						point -= posB;

						// Save point position relative to B position
						glm::vec2 contactPointB = -point;

						// Rotate to B space
						point = glm::vec2(
							point.x * glm::cos(rotB) - point.y * glm::sin(rotB),
							point.x * glm::sin(rotB) + point.y * glm::cos(rotB)
						);

						// Is corner inside a block?
						if (bgB->GetBlock(glm::ivec3(std::round(point.x), 0, std::round(point.y))) != EMPTY)
						{
							// Point within 1 by 1 block
							glm::vec2 rayToBlockCenter = point - glm::vec2(std::round(point.x), std::round(point.y));

							// How far in is this point?
							glm::vec2 rayToClosestCorner(
								(rayToBlockCenter.x > 0.0f ? 0.5f : -0.5f) - rayToBlockCenter.x,
								(rayToBlockCenter.y > 0.0f ? 0.5f : -0.5f) - rayToBlockCenter.y
							);

							// Do not move point into other blocks
							if (bgB->GetBlock(glm::ivec3(std::round(point.x) + 1, 0, std::round(point.y))) != EMPTY)
								rayToClosestCorner.x = std::min(0.0f, rayToClosestCorner.x);
							if (bgB->GetBlock(glm::ivec3(std::round(point.x) - 1, 0, std::round(point.y))) != EMPTY)
								rayToClosestCorner.x = std::max(0.0f, rayToClosestCorner.x);
							if (bgB->GetBlock(glm::ivec3(std::round(point.x), 0, std::round(point.y) + 1)) != EMPTY)
								rayToClosestCorner.y = std::min(0.0f, rayToClosestCorner.y);
							if (bgB->GetBlock(glm::ivec3(std::round(point.x), 0, std::round(point.y) - 1)) != EMPTY)
								rayToClosestCorner.y = std::max(0.0f, rayToClosestCorner.y);

							// Is it still colliding?
							if (glm::length(rayToClosestCorner) != 0.0f)
							{
								// Move shortest distance out of block
								if (rayToClosestCorner.x != 0.0f && rayToClosestCorner.y != 0.0f)
								{
									if (std::abs(rayToClosestCorner.x) < std::abs(rayToClosestCorner.y))
										rayToClosestCorner.y = 0.0f;
									else
										rayToClosestCorner.x = 0.0f;
								}

								// Rotate ray
								glm::vec2 overlap(
									rayToClosestCorner.x * glm::cos(-rotB) - rayToClosestCorner.y * glm::sin(-rotB),
									rayToClosestCorner.x * glm::sin(-rotB) + rayToClosestCorner.y * glm::cos(-rotB)
								);

								glm::vec2 normal = glm::normalize(-overlap);

								// Position correction:

								const float slop = 0.01f; // To prevent resting jitter
								glm::vec2 correction = glm::vec2(std::max(glm::length(overlap) - slop, 0.0f) / (bgA->m_InvMass + bgB->m_InvMass)) * normal;

								bgA->m_PotentialPosition -= bgA->m_InvMass * correction;
								bgB->m_PotentialPosition += bgB->m_InvMass * correction;

								// Impulse response:

								glm::vec2 relativeVelocity =
									(bgB->m_Velocity + CrossProduct(bgB->m_AngularVelocity, contactPointB)) -
									(bgA->m_Velocity + CrossProduct(bgA->m_AngularVelocity, contactPointA));

								float velocityAlongNormal = glm::dot(relativeVelocity, normal);

								if (velocityAlongNormal <= 0.0f)
								{
									const float restitution = 0.5f;

									float rACrossN = CrossProduct(contactPointA, normal);
									float rBCrossN = CrossProduct(contactPointB, normal);
									float invMassSum = bgA->m_InvMass + bgB->m_InvMass
										+ ((rACrossN * rACrossN) * bgA->m_InvInertia)
										+ ((rBCrossN * rBCrossN) * bgB->m_InvInertia);

									float j = -(1.0f + restitution) * velocityAlongNormal;
									j /= invMassSum;

									glm::vec2 impulse = j * normal;

									bgA->m_Velocity += bgA->m_InvMass * -impulse;
									bgB->m_Velocity += bgB->m_InvMass * impulse;

									bgA->m_AngularVelocity += bgA->m_InvInertia * CrossProduct(contactPointA, -impulse);
									bgB->m_AngularVelocity += bgB->m_InvInertia * CrossProduct(contactPointB, impulse);
								}
							}
						}
					}
				}
			}
		}
	}
}
