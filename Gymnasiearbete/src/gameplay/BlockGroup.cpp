#include <queue>

#include "SpriteManager.h"
#include "BlockGroup.h"

BlockGroup::BlockGroup()
{
	m_Model = new Model(GenerateMesh(), "res/textures/blocks.png", "res/shaders/lighting.shader");
}
BlockGroup::~BlockGroup()
{
    
}

void BlockGroup::Update(float deltaTime)
{
    if (m_UpdateNeeded)
    {
        bool empty = true;
        m_Static = false;
        for (int z = -32; z < 32; z++)
        {
            for (int x = -32; x < 32; x++)
            {
                if (GetBlock(glm::ivec3(x, 0, z)) != EMPTY)
                {
                    empty = false;
                }
                if (GetBlock(glm::ivec3(x, 0, z)) == GRASS || GetBlock(glm::ivec3(x, 0, z)) == SAND)
                {
                    m_Static = true;
                }
            }
        }
        
        if (empty && m_OwnedHere)
        {
            for (int z = -32; z < 32; z++)
            {
                for (int x = -32; x < 32; x++)
                {
                    if (GetBlock(glm::ivec3(x, 1, z)) != EMPTY)
                    {
                        BreakBlock(glm::ivec3(x, 1, z));
                    }
                }
            }

            Remove();
            return;
        }

        if (m_OwnedHere) Split();

        if (!m_Static) UpdateMass();

        UpdateRadius();

        m_Model->UpdateData(GenerateMesh());

        m_UpdateNeeded = false;
    }

    if (!m_Static)
    {
        if (m_Bodies.size() > 0) // Delete later
        {
            if (Input::KeyHeld(KEY_UP))
            {
                glm::vec2 acceleration = -glm::vec2(4 * glm::sin(m_Rotation.y), 4 * glm::cos(m_Rotation.y));
                m_Velocity += acceleration * deltaTime;
            }
            if (Input::KeyHeld(KEY_LEFT)) m_AngularVelocity += 1.0f * deltaTime;
            if (Input::KeyHeld(KEY_RIGHT)) m_AngularVelocity -= 1.0f * deltaTime;
        }

        m_PotentialPosition = glm::vec2(m_Position.x, m_Position.z) + m_Velocity * deltaTime;
        m_PotentialRotation = m_Rotation.y + m_AngularVelocity * deltaTime;

        // Delete Later
        m_Velocity *= 0.995f;
        m_AngularVelocity *= 0.995f;
    }
    else
    {
        m_Velocity = glm::vec2(0.0f);
        m_PotentialPosition = glm::vec2(m_Position.x, m_Position.z);
        m_PotentialRotation = m_Rotation.y;
    }

    m_Bodies.clear();

    Sprite::Update(deltaTime);
}

void BlockGroup::OnCollision(Body* body, BlockCollisions side)
{
    if (side == BlockCollisions::Floor && !m_Static)
    {
        if (std::find(m_Bodies.begin(), m_Bodies.end(), body) == m_Bodies.end()) {
            m_Bodies.push_back(body);
        }
    }
}
void BlockGroup::OnCollision(BlockGroup* blockGroup)
{
    // Maybe destroy some blocks if impulse is too great
}

void BlockGroup::Move()
{
    if (m_Static) return;

    glm::vec3 posDelta = glm::vec3(m_PotentialPosition.x, 0.0f, m_PotentialPosition.y) - m_Position;
    float rotDelta = m_PotentialRotation - m_Rotation.y;

    // Apply same movements to child bodies
    for (auto& body : m_Bodies)
    {
        body->m_Position += posDelta;
        body->m_Rotation.y += rotDelta;

        body->m_Position -= glm::vec3(m_PotentialPosition.x, 0.0f, m_PotentialPosition.y);
        body->m_Position = glm::vec3(
            body->m_Position.x * glm::cos(-rotDelta) - body->m_Position.z * glm::sin(-rotDelta),
            body->m_Position.y,
            body->m_Position.x * glm::sin(-rotDelta) + body->m_Position.z * glm::cos(-rotDelta)
        );
        body->m_Position += glm::vec3(m_PotentialPosition.x, 0.0f, m_PotentialPosition.y);
    }
    //m_Bodies.clear();

    m_Position = glm::vec3(m_PotentialPosition.x, 0.0f, m_PotentialPosition.y);

    m_Rotation.y = m_PotentialRotation;
}

void BlockGroup::Draw()
{
    Sprite::Draw();
}

void BlockGroup::UpdateMass()
{
    m_Mass = 0.0f;
    m_InvMass = 0.0f;

    m_Inertia = 1.0f;
    m_InvInertia = 0.0f;

    glm::vec2 CenterOfMass = glm::vec2(0.0f);

    for (int z = -32; z < 32; z++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int x = -32; x < 32; x++)
            {
                if (GetBlock(glm::ivec3(x, y, z)) != EMPTY)
                {
                    m_Mass++;
                    m_Inertia += glm::length(glm::vec2(x, z));

                    CenterOfMass += glm::vec2(x, z);
                }
            }
        }
    }
    m_InvMass = 1.0f / m_Mass;
    m_InvInertia = 1.0f / m_Inertia;

    CenterOfMass /= m_Mass;
    glm::ivec2 iCoM = glm::ivec2(std::round(CenterOfMass.x), std::round(CenterOfMass.y));

    // Move blocks to centre center of mass
    if (iCoM != glm::ivec2(0))
    {
        unsigned char lastBlocks[64][2][64];
        std::memcpy(lastBlocks, m_Blocks, 64 * 2 * 64 * sizeof(unsigned char));
        std::memset(m_Blocks, EMPTY, 64 * 2 * 64 * sizeof(unsigned char));

        for (int z = 0; z < 64; z++)
        {
            for (int y = 0; y < 2; y++)
            {
                for (int x = 0; x < 64; x++)
                {
                    if (x - iCoM.x >= 0 && z - iCoM.y >= 0 && x - iCoM.x < 64 && z - iCoM.y < 64)
                    {
                        m_Blocks[x - iCoM.x][y][z - iCoM.y] = lastBlocks[x][y][z];
                        float l = glm::length(glm::vec2(x - iCoM.x - 32, z - iCoM.y -32));
                        if (l > m_MaxRadius) m_MaxRadius = l;
                    }
                }
            }
        }

        glm::vec2 offset(
            iCoM.x * glm::cos(-m_Rotation.y) - iCoM.y * glm::sin(-m_Rotation.y),
            iCoM.x * glm::sin(-m_Rotation.y) + iCoM.y * glm::cos(-m_Rotation.y)
        );

        m_Position += glm::vec3(offset.x, 0.0f, offset.y);
        m_PotentialPosition += offset;
    }
}

void BlockGroup::UpdateRadius()
{
    m_MaxRadius = 0.0f;
    for (int z = -32; z < 32; z++)
    {
        for (int x = -32; x < 32; x++)
        {
            if (GetBlock(glm::ivec3(x, 0, z)) != EMPTY || GetBlock(glm::ivec3(x, 1, z)) != EMPTY)
            {
                float l = glm::length(glm::vec2(x, z));
                if (l > m_MaxRadius) m_MaxRadius = l;
            }
        }
    }
    m_MaxRadius += 0.707f; 
}

Mesh BlockGroup::GenerateMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int texWidth = m_Model != nullptr ? m_Model->m_Texture.GetWidth() : 16;

    unsigned int index = 0;

    for (int z = -32; z < 32; z++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int x = -32; x < 32; x++)
            {
                if (GetBlock(glm::ivec3(x, y, z)) != EMPTY)
                {
                    for (unsigned int i = 0; i < 10; i++)
                    {
                        // Don't put faces between blocks
                        if (GetBlock(glm::ivec3(x, y + 1, z)) != EMPTY && cubeIndices[i][2] == 1) continue;
                        if (GetBlock(glm::ivec3(x + 1, y, z)) != EMPTY && cubeIndices[i][2] == 2) continue;
                        if (GetBlock(glm::ivec3(x, y, z + 1)) != EMPTY && cubeIndices[i][2] == 3) continue;
                        if (GetBlock(glm::ivec3(x - 1, y, z)) != EMPTY && cubeIndices[i][2] == 4) continue;
                        if (GetBlock(glm::ivec3(x, y, z - 1)) != EMPTY && cubeIndices[i][2] == 5) continue;

                        unsigned int positionIndex[3] = { cubeIndices[i][0], cubeIndices[i][3], cubeIndices[i][6] };
                        unsigned int uvIndex[3] = { cubeIndices[i][1], cubeIndices[i][4], cubeIndices[i][7] };
                        unsigned int normalIndex[3] = { cubeIndices[i][2], cubeIndices[i][5], cubeIndices[i][8] };

                        for (unsigned int ii = 0; ii < 3; ii++)
                        {
                            vertices.push_back(cubePositions[positionIndex[ii] - 1].x + x);
                            vertices.push_back(cubePositions[positionIndex[ii] - 1].y + y - 0.5f);
                            vertices.push_back(cubePositions[positionIndex[ii] - 1].z + z);

                            vertices.push_back((cubeUvs[uvIndex[ii] - 1].x + GetBlock(glm::ivec3(x, y, z)) - 1) * (16.0f / texWidth));
                            unsigned char row = 0;
                            if (normalIndex[ii] == 1)
                                row = 2;
                            else if (GetBlock(glm::ivec3(x, y + 1, z)) == EMPTY)
                                row = 1;
                            vertices.push_back((cubeUvs[uvIndex[ii] - 1].y + row) * (16.0f / 48.0f));

                            vertices.push_back(cubeNormals[normalIndex[ii] - 1].x);
                            vertices.push_back(cubeNormals[normalIndex[ii] - 1].y);
                            vertices.push_back(cubeNormals[normalIndex[ii] - 1].z);

                            indices.push_back(index);
                            index++;
                        }
                    }
                }
            }
        }
    }

    return { vertices, indices };
}

const glm::ivec3 dirs[6] = { 
    glm::ivec3(1, 0, 0),
    glm::ivec3(-1, 0, 0),
    glm::ivec3(0, 1, 0),
    glm::ivec3(0, -1, 0),
    glm::ivec3(0, 0, 1),
    glm::ivec3(0, 0, -1)
};
bool BlockGroup::IsSafe(bool processed[64][2][64], glm::ivec3 coord)
{
    if (coord.x >= 0 && coord.x < 64 && coord.y >= 0 && coord.y < 2 && coord.z >= 0 && coord.z < 64)
    {
        if (m_Blocks[coord.x][coord.y][coord.z] != EMPTY && !processed[coord.x][coord.y][coord.z])
        {
            return true;
        }
    }
    return false;
}
void BlockGroup::FindIsland(bool processed[64][2][64], glm::ivec3 coord)
{
    std::queue<glm::ivec3> q;
    q.push(coord);

    processed[coord.x][coord.y][coord.z] = true;

    while (!q.empty())
    {
        glm::ivec3 c = q.front();
        q.pop();

        for (int i = 0; i < 6; i++)
        {
            if (IsSafe(processed, c + dirs[i]))
            {
                processed[c.x + dirs[i].x][c.y + dirs[i].y][c.z + dirs[i].z] = true;
                q.push(c + dirs[i]);
            }
        }
    }
}
void BlockGroup::Split()
{
    bool processed[64][2][64]{};

    bool found = false;
    bool moreBlocks = false;
    bool newStatic = false;

    BlockGroup* newBG = nullptr;

    for (int z = 0; z < 64; z++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                if (m_Blocks[x][y][z] != EMPTY && processed[x][y][z] == false)
                {
                    if (!found)
                    {
                        FindIsland(processed, glm::ivec3(x, y, z));
                        found = true;
                    }
                    else 
                    {
                        if (!moreBlocks)
                        {
                            newBG = new BlockGroup();
                            newBG->m_Position = m_Position + glm::vec3(randf() * 0.0001f, randf() * 0.0001f, randf() * 0.0001f);
                            newBG->m_Rotation = m_Rotation;
                            newBG->m_Velocity = m_Velocity;
                            newBG->m_AngularVelocity = m_AngularVelocity;
                            newBG->m_UpdateNeeded = true;
                            m_UpdateNeeded = true;
                            moreBlocks = true;
                        }

                        newBG->m_Blocks[x][y][z] = m_Blocks[x][y][z];
                        m_Blocks[x][y][z] = EMPTY;
                    }
                }
            }
        }
    }
    if (newBG != nullptr)
    {
        Update(0.0f);
        newBG->Update(0.0f);
        if (!newBG->WillBeRemoved())
        {
            SpriteManager::AddSprite(newBG);
        }
        else
        {
            delete newBG;
        }
    }
}

void BlockGroup::BreakBlock(glm::ivec3 pos)
{
    glm::vec3 offset(
        pos.x * glm::cos(-m_Rotation.y) - pos.z * glm::sin(-m_Rotation.y),
        pos.y - 0.5f,
        pos.x * glm::sin(-m_Rotation.y) + pos.z * glm::cos(-m_Rotation.y)
    );
    offset += glm::vec3(randf() * 0.05f - 0.025f, 0.0f, randf() * 0.05f - 0.025f);
    DroppedItem* item = new DroppedItem(GetBlock(pos));
    item->m_Position = m_Position + offset;

    //std::cout << "X: " << item->m_Position.x << std::endl;
    //std::cout << "Y: " << item->m_Position.y << std::endl;
    //std::cout << "Z: " << item->m_Position.z << std::endl;
    //std::cout << std::endl;

    SpriteManager::AddSprite(item);

    SetBlock(pos, EMPTY);
}

void BlockGroup::SetBlock(glm::ivec3 pos, unsigned char type)
{
    // Make sure block is within array
    if (pos.x < -32 || pos.y < 0 || pos.z < -32) return;
    if (pos.x >= 32 || pos.y >= 2 || pos.z >= 32) return;

    m_Blocks[pos.x + 32][pos.y][pos.z + 32] = type;

    m_UpdateNeeded = true;
}
char BlockGroup::GetBlock(glm::ivec3 pos)
{
    // Make sure block is within array
    if (pos.x < -32 || pos.y < 0 || pos.z < -32) return EMPTY;
    if (pos.x >= 32 || pos.y >= 2 || pos.z >= 32) return EMPTY;

    return m_Blocks[pos.x + 32][pos.y][pos.z + 32];
}

void BlockGroup::SetDescription(std::vector<uint8_t>& desc)
{
    unsigned char lastBlocks[64][2][64];
    std::memcpy(lastBlocks, m_Blocks, 64 * 2 * 64 * sizeof(unsigned char));

    desc >> m_WillBeRemoved >> m_Static >> m_AngularVelocity >> m_Velocity >> m_Blocks >> m_Scale >> m_Rotation >> m_Position; // m_Static should not be here

    if (std::memcmp(lastBlocks, m_Blocks, 64 * 2 * 64 * sizeof(unsigned char)) != 0.0f) m_UpdateNeeded = true;
}
std::vector<uint8_t> BlockGroup::GetDescription() const
{
    std::vector<uint8_t> desc;
    desc << m_Position << m_Rotation << m_Scale << m_Blocks << m_Velocity << m_AngularVelocity << m_Static << m_WillBeRemoved; // m_Static should not be here
    return desc;
}