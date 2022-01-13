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

	static void BlocksToBlocks(BlockGroup* blockGroup1, BlockGroup* blockGroup2, float deltaTime)
	{
		BlockGroup* blockGroupA = blockGroup1;
		BlockGroup* blockGroupB = blockGroup2;

		for (int i = 0; i < 2; i++)
		{
			if (i == 1)
			{
				blockGroupA = blockGroup2;
				blockGroupB = blockGroup1;
			}

			glm::vec2 posA = blockGroupA->m_PotentialPosition;
			glm::vec2 posB = blockGroupB->m_PotentialPosition;

			float rotA = blockGroupA->m_PotentialRotation;
			float rotB = blockGroupB->m_PotentialRotation;

			for (int z = -32; z < 32; z++)
			{
				for (int x = -32; x < 32; x++)
				{
					if (blockGroupA->GetBlock(glm::vec3(x, 0, z)) != EMPTY || blockGroupA->GetBlock(glm::vec3(x - 1, 0, z - 1)) != EMPTY)
					{
						glm::vec2 point(x - 0.5f, z - 0.5f);

						point = glm::vec2(
							point.x * glm::cos(-rotA) - point.y * glm::sin(-rotA),
							point.x * glm::sin(-rotA) + point.y * glm::cos(-rotA)
						);

						point += posA;

						point -= posB;

						point = glm::vec2(
							point.x * glm::cos(rotB) - point.y * glm::sin(rotB),
							point.x * glm::sin(rotB) + point.y * glm::cos(rotB)
						);

						for (int z = round(point.y) - 2; z < round(point.y) + 2; z++)
						{
							for (int x = round(point.x) - 2; x < round(point.x) + 2; x++)
							{
								if (blockGroupB->GetBlock(glm::vec3(x, 0, z)) != EMPTY)
								{
									if (point.x > x - 0.5f && point.x < x + 0.5f && point.y > z - 0.5f && point.y < z + 0.5f)
									{
										glm::vec2 overlap = glm::vec2(x, z) - point;

										if (std::abs(overlap.x) < std::abs(overlap.y))
										{
											overlap.x = 0.0f;
											overlap.y += overlap.y < 0.0f ? -0.5f : 0.5f;
										}
										else
										{
											overlap.y = 0.0f;
											overlap.x += overlap.x < 0.0f ? -0.5f : 0.5f;
										}
										overlap = glm::vec2(
											overlap.x * glm::cos(-rotB) - overlap.y * glm::sin(-rotB),
											overlap.x * glm::sin(-rotB) + overlap.y * glm::cos(-rotB)
										);

										blockGroupA->m_PotentialPosition -= overlap * 0.5f; // Scale by mass instead
										blockGroupB->m_PotentialPosition += overlap * 0.5f;

										//blockGroupA->m_Velocity -= overlap * 0.5f;
										//blockGroupB->m_Velocity += overlap * 0.5f;
									}


									/*
									glm::vec2 nearestPoint;
									nearestPoint.x = std::max(float(x - 0.5f), std::min(float(x + 0.5f), point.x));
									nearestPoint.y = std::max(float(z - 0.5f), std::min(float(z + 0.5f), point.y));

									glm::vec2 rayToNearest = nearestPoint - point;
									float overlap = 0.1f - glm::length(rayToNearest);

									if (std::isnan(overlap)) overlap = 0.0f;

									if (overlap > 0.0f && glm::length(rayToNearest) > 0.0f)
									{
										rayToNearest = glm::vec2(
											rayToNearest.x * glm::cos(-rotB) - rayToNearest.y * glm::sin(-rotB),
											rayToNearest.x * glm::sin(-rotB) + rayToNearest.y * glm::cos(-rotB)
										);

										blockGroupA->m_PotentialPosition -= rayToNearest * 0.5f; // Scale by mass instead
										blockGroupB->m_PotentialPosition += rayToNearest * 0.5f;

										blockGroupA->m_Velocity -= rayToNearest * 0.5f;
										blockGroupB->m_Velocity += rayToNearest * 0.5f;
									}
									*/
								}
							}
						}
					}
				}
			}
		}
	}
}
