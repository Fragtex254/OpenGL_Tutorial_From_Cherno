#pragma once

#include "Test.h"

namespace Test {
	class TestClearColor: public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];

	};
}

