#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>

namespace Test 
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test(){}

		/*Some Note Here
		why don't use pure virtual function here is because we don't want to strictly fill all the current test class function up.
		BTW if we are making a series engine system we'd better to override all the pure virtual function up.*/

		virtual void OnUpdate(float deltaTime){}
		virtual void OnRender(){}
		virtual void OnImGuiRender(){}

	};


	class TestMenu :public Test {

	public:
		TestMenu(Test*& currentTestPtr);

		void OnImGuiRender()override;
		template<typename T>
		void RegisterTest(const std::string& name) 
		{
			std::cout << "Registering test" << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}

