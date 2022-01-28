#include "WindowApp.h"
#include "../VulkanSetup/Input.h"
#include "Coordinate.h"
#include "Math.h"
#include "Triangulation.h"
#include "GUIEx.h"

#include "../VulkanSetup/AnimationLoader.h"
#include "../VulkanSetup/AnimationDataPack.h"
#include "../VulkanSetup/AnimationDataRow.h"
#include "../VulkanSetup/Transform.h"
#include "../VulkanSetup/MathEx.h"

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

VOID onPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255,0,0,255));
	graphics.DrawLine(&pen, 0, 0, 200, 100);
}

int main()
{
	WindowApp app(1280,600);
	Input* input = Input::getInstance();
	Coordinate coord;
	coord.createCoordinate(Color(255, 220, 220, 220), 640,300,1280,600);

	RECT rect;
	GetClientRect(app.getHWND(), &rect);

	Bitmap bmp(rect.right, rect.bottom);
	Graphics memDC(&bmp);
	SolidBrush brush(Color(255, 255, 255, 255));

	//AnimationLoader loader;
	//AnimationDataRow* row = loader.loadAnimationBinary("../VulkanSetup/Resource/ani_idle.dat");
	//AnimationDataRow* realRow = loader.loadAnimation("../RowResource/Animation/ani_idle.pa");
	//AnimationDataPack* smallPack = new AnimationDataPack;
	//smallPack->createAnimationDataPack(row, "ani_idle");


	GUIEx gui;
	gui.createGUI(&memDC);

	//auto& bones = row->_bones;

	//bool pick = false;
	//float scrollY = 0.f;
	//float scaleY = 1.f;

	//BoneDataRow* graphData = nullptr;
	//BoneDataRow* debugData = nullptr;
	//size_t currentName;

	Triangulation tri;
	tri.createTriangulation();
	tri.triangulation();
	bool pick = false;


	while (app.peekMessageCheck() != MessageState::M_QUIT)
	{
		if (app.getRecentlyState() == MessageState::M_FALSE)
		{
			Sleep(10);

			POINT p;
			GetCursorPos(&p);
			ScreenToClient(app.getHWND(), &p);

			if (input->isDown(VK_LEFT))
				coord.addCenterX(20);
			if (input->isDown(VK_RIGHT))
				coord.addCenterX(-20);
			if (input->isDown(VK_UP))
				coord.addCenterY(20);
			if (input->isDown(VK_DOWN))
				coord.addCenterY(-20);

			if (input->isDown('Q'))
			{
				coord.addRatioX(1);
				coord.addRatioY(1);
			}
			if (input->isDown('A'))
			{
				coord.addRatioX(-1);
				coord.addRatioY(-1);
			}


			//if (input->isDown('E'))
			//{
			//	scrollY += 10.f;
			//	scrollY = scrollY >= 0.f ? 0.f : scrollY;
			//}
			//if (input->isDown('D'))
			//{
			//	scrollY -= 10.f;
			//	
			//}

			input->updateMousePosition(p.x, p.y);

			if (input->isDown('S') && !pick)
			{
				XMFLOAT2 world = coord.toWorldSpace(XMFLOAT2{ (float)p.x,(float)p.y });
				tri.addPoint(world.x, world.y);
				tri.triangulation();

				pick = true;
			}
			else if (input->isUp('S') && pick)
			{
				pick = false;
			}

			HDC hdc = GetDC(app.getHWND());
			Graphics graphics(hdc);
			memDC.FillRectangle(&brush, Rect(0,0,1280,600));

			coord.draw(&memDC);
			
			auto worldPoint = coord.toWorldSpace(XMFLOAT2{ (float)p.x,(float)p.y });
			coord.drawRect(&memDC, Color(255, 0, 255, 0), worldPoint.x, worldPoint.y, 10, 10);


			//gui.button(Color(255, 0, 0, 0), L"wdw", 100, 30, 100, 15);

			/*int i = 0;
			for (auto iterator = bones.begin(); iterator != bones.end(); ++iterator)
			{
				std::wstring wide = std::wstring(iterator->second._name.begin(), iterator->second._name.end());

				++i;

				float startY = 20 * i + 20 + scrollY;
				if(startY < 0.f || startY >= 600.f)
					continue;
				if (gui.button(Color(255, 0, 0, 0), wide.c_str(), 120, startY, 100, 15))
				{
					graphData = &iterator->second;
					debugData = &realRow->_bones[iterator->first];
					currentName = iterator->first;
				}
				
			}
			

			if (graphData != nullptr)
			{
				float avg = 0.f;
				for (int i = 1; i < graphData->_frames.size(); ++i)
				{
					float px = static_cast<float>(graphData->_frames[i-1].frame) * 0.1f;
					float py = graphData->_frames[i-1].rotation._w;

					float x = static_cast<float>(graphData->_frames[i].frame) * 0.1f;
					float y = graphData->_frames[i].rotation._w;

					coord.drawLine(&memDC, Color(100, 255, 0, 0), px, py * scaleY, x, y * scaleY,false);
				}

				

				for (int i = 1; i < debugData->_frames.size(); ++i)
				{
					float px = static_cast<float>(debugData->_frames[i - 1].frame) * 0.1f;
					float py = debugData->_frames[i - 1].rotation._w;

					float x = static_cast<float>(debugData->_frames[i].frame) * 0.1f;
					float y = debugData->_frames[i].rotation._w;

					int outIndex;
					float pt = (float)debugData->_frames[i].frame / (float)smallPack->getFrameCount();
					XMFLOAT4 realRot;
					XMStoreFloat4(&realRot, smallPack->getPoseByPercentage(outIndex, pt, currentName).getRotation());
					avg += 1.f - MathEx::abs(realRot.w - y) / 2.f;

					coord.drawLine(&memDC, Color(100, 0, 0, 255), px, py * scaleY, x, y * scaleY, false);
				}

				auto wf = std::to_wstring(avg / (debugData->_frames.size() - 1));
				coord.drawString(&memDC, Color(255, 0, 0, 0), wf.c_str(), 10, 100);
			}*/

			auto& points = tri.getPoints();
			auto& triangles = tri.getTriangles();

			for (int i = 0; i < triangles.size(); ++i)
			{
				auto a = points[triangles[i]._a];
				auto b = points[triangles[i]._b];
				auto c = points[triangles[i]._c];

				float ots;
				float t;
				float s;
				float nearPointDistance;
				XMVECTOR nearestPoint;
				
				bool inTriangle = !Math::findNearestPointOnTriangle(XMVectorSet(worldPoint.x, worldPoint.y, 0.f, 0.f),
					XMVectorSet(a.x, a.y, 0.f, 0.f), XMVectorSet(b.x, b.y, 0.f, 0.f), XMVectorSet(c.x, c.y, 0.f, 0.f),
					nearestPoint, nearPointDistance, t, s, ots);

				Color red = Color(255, 255, 0, 0);
				Color green = Color(255, 0, 255, 0);
	
				coord.drawLine(&memDC, inTriangle ? green : red, a.x, a.y, b.x, b.y);
				coord.drawLine(&memDC, inTriangle ? green : red, b.x, b.y, c.x, c.y);
				coord.drawLine(&memDC, inTriangle ? green : red, c.x, c.y, a.x, a.y);

				if (inTriangle == false)
				{
					XMFLOAT3 stored;
					XMStoreFloat3(&stored, nearestPoint);
					coord.drawRect(&memDC, Color(255, 0, 255, 0), stored.x, stored.y, 10, 10);

					coord.drawLine(&memDC, Color(255, 0, 255, 0), stored.x, stored.y, worldPoint.x, worldPoint.y);
				}

				
			}

			
			
			graphics.DrawImage(&bmp, 0, 0);
			
			


			ReleaseDC(app.getHWND(), hdc);

		}
		else if (app.getRecentlyState() == MessageState::M_PEEK)
		{
			MSG msg = *app.getMessagePointer();
			if (msg.message == WM_KEYDOWN)
			{
				input->setKeyDown(static_cast<unsigned int>(msg.wParam));
			}
			else if (msg.message == WM_KEYUP)
			{
				input->setKeyUp(static_cast<unsigned int>(msg.wParam));
			}
			else if (msg.message == WM_LBUTTONDOWN)
			{
				input->setKeyDown(static_cast<unsigned int>(VK_LBUTTON));
			}
			else if (msg.message == WM_LBUTTONUP)
			{
				input->setKeyUp(static_cast<unsigned int>(VK_LBUTTON));
			}

		}
	}

	//delete row;

	return 0;
}