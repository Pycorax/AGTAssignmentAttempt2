#include "UIButton.h"
#include "../Application.h"

UIButton::UIButton()
	: m_mesh(NULL)
	, m_state(UP_STATE)
{
}


UIButton::~UIButton()
{
}

void UIButton::Init(Mesh * mesh, Vector3 pos, Vector3 scale)
{
	m_mesh = mesh;
	m_pos = pos;
	m_state = UP_STATE;
	m_scale = scale;
}

void UIButton::UpdateState(bool pressed, float windowHeight)
{
	// Get cursor position...
	double x = Application::mouse_current_x;

	// ...and convert Screen Space into World Space
	double y = windowHeight - Application::mouse_current_y;

	// Checking if cursor is on the mouse
	if (
		x >= m_pos.x - m_scale.x * 0.5f && x <= m_pos.x + m_scale.x * 0.5f
		&&
		y >= m_pos.y - m_scale.y * 0.5f && y <= m_pos.y + m_scale.y * 0.5f
		)
	{
		if (pressed)	// If use clicked
		{
			m_state = DOWN_STATE;
		}
		else
		{
			m_state = HOVER_STATE;
		}
	}
	else	// Cursor not on mouse
	{
		m_state = UP_STATE;
	}
}

void UIButton::SetMesh(Mesh * mesh)
{
	m_mesh = mesh;
}

Mesh * UIButton::GetMesh(void)
{
	return m_mesh;
}

Vector3 UIButton::GetPosition(void)
{
	return m_pos;
}

Vector3 UIButton::GetScale()
{
	return m_scale;
}

UIButton::BUTTON_STATE_TYPE UIButton::GetState(void)
{
	return m_state;
}
