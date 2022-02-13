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


#include "../VulkanSetup/SplineKeyVariable.h"
#include "../BinaryGenerator/FBXLoader.h"

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

	AnimationLoader loader;
	FBXLoader fbxLoader;
	AnimationDataRow* row = loader.loadAnimationBinary("../Resource/Ani_Player_Run_Tilt_R_04.dat");
	AnimationDataRow* realRow = fbxLoader.loadAnimation("../Resource/Ani_Player_Run_Tilt_R_04.fbx");
	AnimationDataPack* smallPack = new AnimationDataPack;
	AnimationDataPack* realPack = new AnimationDataPack;
	smallPack->createAnimationDataPack(row, "ani_idle");
	realPack->createAnimationDataPack(realRow, "ani_idle");

	GUIEx gui;
	gui.createGUI(&memDC);

	auto& bones = row->_hashBones;

	bool pick = false;
	float scrollY = 0.f;
	float scaleY = 1.f;

	size_t currentName = -1;

	//Triangulation tri;
	//tri.createTriangulation();
	//tri.triangulation();
	//bool pick = false;

	float recentlyX = 0.f;
	float recentlyY = 0.f;
	SplineKeyVariable spline;

	while (app.peekMessageCheck() != MessageState::M_QUIT)
	{
		if (app.getRecentlyState() == MessageState::M_FALSE)
		{
			Sleep(49);

			POINT p;
			GetCursorPos(&p);
			ScreenToClient(app.getHWND(), &p);
#pragma region keyInput
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
				coord.addRatioX(10);
			}
			if (input->isDown('A'))
			{
				coord.addRatioX(-10);
			}

			if (input->isDown('W'))
			{
				coord.addRatioY(10);
			}
			if (input->isDown('S'))
			{
				coord.addRatioY(-10);
			}


			if (input->isDown('E'))
			{
				scrollY += 10.f;
				scrollY = scrollY >= 0.f ? 0.f : scrollY;
			}
			if (input->isDown('D'))
			{
				scrollY -= 10.f;

			}


			input->updateMousePosition(p.x, p.y);

			if (input->isDown('F') && !pick)
			{
				XMFLOAT2 world = coord.toWorldSpace(XMFLOAT2{ (float)p.x,(float)p.y });
				spline.insertPoint(world.y, world.x);

				pick = true;
			}
			else if (input->isUp('F') && pick)
			{
				pick = false;
			}

#pragma endregion

			HDC hdc = GetDC(app.getHWND());
			Graphics graphics(hdc);
			memDC.FillRectangle(&brush, Rect(0,0,1280,600));

			coord.draw(&memDC);
			
			auto worldPoint = coord.toWorldSpace(XMFLOAT2{ (float)p.x,(float)p.y });
			coord.drawRect(&memDC, Color(255, 0, 255, 0), worldPoint.x, worldPoint.y, 10, 10);

			

			for (int i = 0; i < spline.getSize() - 1; ++i)
			{
				for (int j = 0; j < 10; ++j)
				{
					XMVECTOR start = spline.getSpline(i, (float)j / 10.f);
					XMVECTOR end = spline.getSpline(i, (float)(j + 1) / 10.f);

					XMFLOAT3 sf;
					XMFLOAT3 ef;

					XMStoreFloat3(&sf, start);
					XMStoreFloat3(&ef, end);

					coord.drawLine(&memDC, Color(255, 255, 0, 0), sf.x, sf.y, ef.x, ef.y, false);
				}
			}


			 int k = 0;
			auto& table = bones.getTable();
			for (size_t i = 0; i < table.size(); ++i)
			{
				for (size_t j = 0; j < table[i].size(); ++j)
				{
					std::wstring wide = std::wstring(table[i][j]._data._name.begin(), table[i][j]._data._name.end());

					++k;

					float startY = 20 * k + 20 + scrollY;
					if (startY < 0.f || startY >= 600.f)
						continue;
					if (gui.button(Color(255, 0, 0, 0), wide.c_str(), 120, startY, 100, 15))
					{
						currentName = table[i][j]._key;

						spline.clearPoint();
						spline.addPoint(XMVectorGetW(realPack->getPoseByIndex(0, currentName).getRotation()), 0.f);

						float interval = 1.f / (float)realPack->getFps();

						FrameData* data = &(*(table[i][j]._data._frames.end() - 1));
						interval = interval * data->frame;
						spline.addPoint(data->rotation.getQuaternion().w, interval);
					}
				}
			}


			/*for (auto iterator = table.begin(); iterator != table.end(); ++iterator)
			{
				std::wstring wide = std::wstring((*iterator)._data._name.begin(), iterator->second._name.end());

				++i;

				float startY = 20 * i + 20 + scrollY;
				if(startY < 0.f || startY >= 600.f)
					continue;
				if (gui.button(Color(255, 0, 0, 0), wide.c_str(), 120, startY, 100, 15))
				{
					currentName = iterator->first;

					spline.clearPoint();
					spline.addPoint(XMVectorGetW(realPack->getPoseByIndex(0,currentName).getRotation()), 0.f);

					float interval = 1.f / (float)realPack->getFps();

					FrameData* data = &(*((*iterator).second._frames.end() - 1));
					interval = interval * data->frame;
					spline.addPoint(data->rotation.getQuaternion().w, interval);
				}
				
			}*/
			
			bool iteration = false;
			if (input->isDown(VK_RETURN))
			{
				iteration = true;
			}


			if (currentName != -1)
			{
				float interval = 0.01666f;
				float time = realPack->getTime();
				float timer = 0.f;

				/*float biggestDiff = 0.f;
				float diffVal = 0.f;
				float diffTime = 0.f;*/

				while (true)
				{
					size_t outIndex;
					auto pose = XMVectorGetW(realPack->getPoseByTime(outIndex, timer, currentName).getRotation());
					float save = timer;
					timer += interval;
					auto pose2 = XMVectorGetW(realPack->getPoseByTime(outIndex, timer, currentName).getRotation());

					coord.drawLine(&memDC, Color(255, 0, 255, 0), save, pose, timer, pose2, false);

					if (timer >= time)
					{
						break;
					}
				}

				if (iteration)
				{
					float biggestDiff = 0.f;
					int startIndex = 0;

					for (int i = 0; i < spline.getSize() - 1; ++i)
					{
						float startTime = spline.getPointTime(i);
						float endTime = spline.getPointTime(i + 1);
						float diff = 0.f;
						float deltaTime = 0.01666f;
						timer = startTime;
						float percentage = timer / time;
						bool diffCheck = false;
						while (true)
						{
							size_t outIndex;
							auto pose = XMVectorGetW(realPack->getPoseByTime(outIndex, timer, currentName).getRotation());
							percentage = timer / time;

							float splineValue = spline.getValueByPercentage(percentage);
							float diffrence = MathEx::abs(pose - splineValue);
							if (diffrence >= 0.0005f)
								diffCheck = true;
							diff += diffrence;

							timer += deltaTime;
							if (timer >= endTime)
								break;
						}

						if (diff > biggestDiff)
						{
							startIndex = i;
							biggestDiff = diff;
						}

						if (diffCheck == false)
							int wer = 0;
					}

					size_t outIndex;
					float insertTime = spline.getPointTime(startIndex) + (spline.getPointTime(startIndex + 1) - spline.getPointTime(startIndex)) * 0.5f;
					float rotationData = XMVectorGetW(realPack->getPoseByTime(outIndex, insertTime, currentName).getRotation());
					
					recentlyX = insertTime;
					recentlyY = rotationData;

					spline.insertPoint(rotationData, insertTime);
					//spline.insertPoint(diffVal, diffTime);
				}

			}

			coord.drawRect(&memDC, Color(100, 255, 0, 0), recentlyX, recentlyY, 10.f, 10.f);

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