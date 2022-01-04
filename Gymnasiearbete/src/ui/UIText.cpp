#include "UIText.h"

UIText::UIText()
{
    m_Image = new Image("res/images/font.png");
    SetText("");
}
UIText::UIText(const std::string& text)
{
    m_Image = new Image("res/images/font.png");
	SetText(text);
}
UIText::~UIText()
{

}

void UIText::SetText(const std::string& text)
{
    if (text != m_Text)
    {
        m_Text = text;

        m_Mesh = GenerateMesh();

        m_Image->UpdateData(m_Mesh);
    }
}
std::string UIText::GetText() const
{
	return m_Text;
}

Mesh UIText::GenerateMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int index = 0;

    for (unsigned int c = 0; c < m_Text.length(); c++)
    {
        unsigned int symbol = (int)m_Text[c] - 32;

        for (unsigned int i = 0; i < 2; i++)
        {
            unsigned int positionIndex[3] = { squareIndices[i][0], squareIndices[i][2], squareIndices[i][4] };
            unsigned int uvIndex[3] = { squareIndices[i][1], squareIndices[i][3], squareIndices[i][5] };

            for (unsigned int ii = 0; ii < 3; ii++)
            {
                vertices.push_back(squarePositions[positionIndex[ii] - 1].x + symbolWidth * c);
                vertices.push_back(squarePositions[positionIndex[ii] - 1].y);

                vertices.push_back((squareUvs[uvIndex[ii] - 1].x + symbol) * symbolWidth / m_Image->GetWidth());
                vertices.push_back(squareUvs[uvIndex[ii] - 1].y);

                indices.push_back(index);
                index++;
            }
        }
    }

    return { vertices, indices };
}