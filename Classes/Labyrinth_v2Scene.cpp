/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "Labyrinth_v2Scene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;

Scene* Labyrinth::createScene()
{
	return Labyrinth::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Labyrinth::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	labyrinthDN = DrawNode::create();
	startDN = DrawNode::create();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(Labyrinth::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...
	createCage();
	labyrinthDN = DrawNode::create();
	drawCage();
	labyrinthDN->setPosition(origin.x + CELL_SIZE, origin.y + CELL_SIZE);
	this->addChild(labyrinthDN);
	startDN->setPosition(origin.x + CELL_SIZE, origin.y + CELL_SIZE);
	this->addChild(startDN);


	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Labyrinth::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//this->scheduleUpdate();
	return true;
}

void Labyrinth::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {

	switch (keyCode) {
		//if Space was pressed
	case EventKeyboard::KeyCode::KEY_SPACE:
	{
		//if the length of the way is too small and we are not on the borderline, so can't get out
	//	if (length < SIZE + 1 || (strStart != 0 && strStart != SIZE - 1 && stlbStart != 0 && stlbStart != SIZE - 1))
	//	{
		findWay(strStart, stlbStart);
		labyrinthDN->clear();
		drawCage();
		vector<Vec2>haveNeibs, vector<Vec2>neibs;
		for (int i = 0; i < way.size(); i++)
		{
			neibs = findNeibs(way[i].x, way[i].y);
			if (neibs.size() > 0)
				haveNeibs.push_back(way[i]);
		}
		int row = 0;

		//choose random new start cell
		row = random(0, static_cast<int>(haveNeibs.size()) - 1);
		strStart = haveNeibs[row].x;
		stlbStart = borders[row].y;




		//	}
			//else //if the way was found
			//{
			//	way.clear(); //clear the way the find the new one
			//	unvisited.clear();
			//	getUnvizited();

			//	if (unvisited.size() > 0)
			//	{
			//		//findWay();
			//	}
			//	else
			//	{
			//		way.clear();
			//		labyrinthDN->clear();
			//		drawCage();
			//	}
			//}
		break;
	}
	default:
		break;
	}
}

//generate labyrinth
void Labyrinth::createCage()
{
	int b0, b1, b2, b3; //bits

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)

		{
			int str = SIZE - 1 - i, stlb = j; //for the indexes to correspond the coordinates
			//if the point is on the border
			if (str == 0 || stlb == 0 || str == SIZE - 1 || stlb == SIZE - 1)
			{
				//extreme left
				if (str == 0)
				{
					//lower left corner
					if (stlb == 0)
					{
						b0 = 1;
						b1 = random(0, 1);
						b2 = random(0, 1);
						b3 = 1;
					}
					//upper left corner
					else if (stlb == SIZE - 1)
					{
						b0 = 1;
						b1 = 1;
						b2 = random(0, 1);
						b3 = random(0, 1);
					}
					else //just extreme left
					{
						b0 = 1;
						b1 = random(0, 1);
						b2 = random(0, 1);
						b3 = random(0, 1);
					}
				}
				//extreme low
				else if (stlb == 0)
				{
					//lower right corner
					if (str == SIZE - 1)
					{
						b0 = random(0, 1);
						b1 = random(0, 1);
						b2 = 1;
						b3 = 1;
					}
					//just extreme low
					else
					{
						b0 = random(0, 1);
						b1 = random(0, 1);
						b2 = random(0, 1);
						b3 = 1;
					}
				}
				//extreme right
				else if (str == SIZE - 1)
				{
					//upper right corner
					if (stlb == SIZE - 1)
					{
						b0 = random(0, 1);
						b1 = 1;
						b2 = 1;
						b3 = random(0, 1);
					}
					//just extreme right
					else
					{
						b0 = random(0, 1);
						b1 = random(0, 1);
						b2 = 1;
						b3 = random(0, 1);
					}
				}
				//extreme up
				else if (stlb == SIZE - 1)
				{
					b0 = random(0, 1);
					b1 = 1;
					b2 = random(0, 1);
					b3 = random(0, 1);
				}
				labyrinth.cells[str][stlb] = 8 * b0 + 4 * b1 + 2 * b2 + 1 * b3;
			}
			//somewhere in the middle
			else
				labyrinth.cells[str][stlb] = random(0, 15);
		}
	}

	//get border cells
	vector<Vec2> borders;
	int row = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (i == 0 || j == 0 || i == (SIZE - 1) || j == (SIZE - 1))
			{
				borders.push_back(Vec2(i, j));
			}
		}
	}
	//choose random start cell
	row = random(0, static_cast<int>(borders.size()) - 1);
	strStart = SIZE - 1 - borders[row].x;
	stlbStart = borders[row].y;
}

//draw labyrinth
void Labyrinth::drawCage()
{
	//draw the way
	if (way.size() > 1)
	{
		for (int i = 0; i < way.size() - 1; i++)
		{
			labyrinthDN->drawLine(Vec2(CELL_SIZE * (way[i].x) + CELL_SIZE / 2, CELL_SIZE * (way[i].y) + CELL_SIZE / 2), Vec2(CELL_SIZE * (way[i + 1].x) + CELL_SIZE / 2, CELL_SIZE * (way[i + 1].y) + CELL_SIZE / 2), Color4F(34.0, 139.0, 34.0, 1.0));
			//arrow
			//labyrinthDN->drawLine(Vec2(CELL_SIZE * (way[i+1].x)-3 + CELL_SIZE / 2, CELL_SIZE * (way[i+1].y)+6 + CELL_SIZE / 2), Vec2(CELL_SIZE * (way[i + 1].x)-3 + CELL_SIZE / 2, CELL_SIZE * (way[i + 1].y)-6 + CELL_SIZE / 2), Color4F(34.0, 139.0, 34.0, 1.0));
			//labyrinthDN->drawLine(Vec2(CELL_SIZE * (way[i+1].x)-3 + CELL_SIZE / 2, CELL_SIZE * (way[i+1].y)-6 + CELL_SIZE / 2), Vec2(CELL_SIZE * (way[i + 1].x) + CELL_SIZE / 2, CELL_SIZE * (way[i + 1].y) + CELL_SIZE / 2), Color4F(34.0, 139.0, 34.0, 1.0));
			labyrinthDN->drawLine(Vec2(CELL_SIZE * (way[i + 1].x) - 3 + CELL_SIZE / 2, CELL_SIZE * (way[i + 1].y) + 6 + CELL_SIZE / 2), Vec2(CELL_SIZE * (way[i + 1].x) + CELL_SIZE / 2, CELL_SIZE * (way[i + 1].y) + CELL_SIZE / 2), Color4F(34.0, 139.0, 34.0, 1.0));
		}
	}
	//go through the cells
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			int str = SIZE - 1 - i, stlb = j; //for the indexes to correspond the coordinates
			int temp = labyrinth.cells[str][stlb]; //for bit shift
			if (str == strStart && stlb == stlbStart) //if the current cell is the start cell
			{
				startDN->drawPoint(Vec2(CELL_SIZE * str + CELL_SIZE / 2, CELL_SIZE * stlb + CELL_SIZE / 2), 10, Color4F(34.0, 139.0, 34.0, 1.0));
			}
			//go through the cell bits
			for (int k = 3; temp != 0; temp >>= 1, k--)
			{
				//if there's a wall
				if ((temp & 1) == 1)
				{
					//draw the current wall
					switch (k)
					{
					case 0://right
						labyrinthDN->drawLine(Vec2(CELL_SIZE * str, CELL_SIZE * stlb), Vec2(CELL_SIZE * str, CELL_SIZE * (stlb + 1)), Color4F(1.0, 1.0, 1.0, 1.0));
						break;
					case 1://up
						labyrinthDN->drawLine(Vec2(CELL_SIZE * str, CELL_SIZE * (stlb + 1)), Vec2(CELL_SIZE * (str + 1), CELL_SIZE * (stlb + 1)), Color4F(1.0, 1.0, 1.0, 1.0));
						break;
					case 2://left
						labyrinthDN->drawLine(Vec2(CELL_SIZE * (str + 1), CELL_SIZE * (stlb + 1)), Vec2(CELL_SIZE * (str + 1), CELL_SIZE * stlb), Color4F(1.0, 1.0, 1.0, 1.0));
						break;
					case 3://down
						labyrinthDN->drawLine(Vec2(CELL_SIZE * str, CELL_SIZE * stlb), Vec2(CELL_SIZE * (str + 1), CELL_SIZE * stlb), Color4F(1.0, 1.0, 1.0, 1.0));
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

//find the way from start point
void Labyrinth::findWay(int x, int y)
{
	if (gone[x][y] == 1)
		return;
	else
	{
		gone[x][y] = 1;
		way.push_back(Vec2(x, y));

		if ((x < SIZE - 1) && (!isWall(x, y, x + 1, y)))
			findWay(x + 1, y);

		if ((x > 0) && !isWall(x, y, x - 1, y))
			findWay(x - 1, y);

		if ((y < SIZE - 1) && !isWall(x, y, x, y + 1))
			findWay(x, y + 1);

		if ((y > 0) && !isWall(x, y, x, y - 1))
			findWay(x, y - 1);
	}
}

//returns the next point for the current point
vector<Vec2> Labyrinth::findNeibs(int strStart, int stlbStart)
{
	int row = 0;
	vector<Vec2> neibs;

	if ((strStart > 0) && (gone[strStart - 1][stlbStart] == 0))
		neibs.push_back(Vec2(strStart - 1, stlbStart));
	if ((strStart < SIZE - 1) && (gone[strStart + 1][stlbStart] == 0))
		neibs.push_back(Vec2(strStart + 1, stlbStart));

	if ((stlbStart > 0) && (gone[strStart][stlbStart - 1] == 0))
		neibs.push_back(Vec2(strStart, stlbStart - 1));
	if ((stlbStart < SIZE - 1) && (gone[strStart][stlbStart + 1] == 0))
		neibs.push_back(Vec2(strStart, stlbStart + 1));
	return neibs;
}

//delete current wall of current cell
int Labyrinth::deleteWall(int cell, int pos)
{
	unsigned mask = 1;

	if (0 <= pos && pos < 4)
		return cell & (~(mask << pos));
	return cell;
}

//go from current cell to the next cell
void Labyrinth::goThroughWalls(int strStart, int stlbStart, int strEnd, int stlbEnd)
{
	if (strStart > strEnd) //go right
	{
		//delete the left wall of the start cell
		labyrinth.cells[strStart][stlbStart] = deleteWall(labyrinth.cells[strStart][stlbStart], 3);
		//delete the right wall of the end cell
		labyrinth.cells[strEnd][stlbEnd] = deleteWall(labyrinth.cells[strEnd][stlbEnd], 1);
	}
	else if (strStart < strEnd) //go left
	{
		//delete the right wall of the start cell
		labyrinth.cells[strStart][stlbStart] = deleteWall(labyrinth.cells[strStart][stlbStart], 1);
		//delete the left wall of the end cell
		labyrinth.cells[strEnd][stlbEnd] = deleteWall(labyrinth.cells[strEnd][stlbEnd], 3);
	}
	else if (stlbStart > stlbEnd) //go down
	{
		//delete the down wall of the start cell
		labyrinth.cells[strStart][stlbStart] = deleteWall(labyrinth.cells[strStart][stlbStart], 0);
		//delete the up wall of the end cell
		labyrinth.cells[strEnd][stlbEnd] = deleteWall(labyrinth.cells[strEnd][stlbEnd], 2);
	}
	else if (stlbStart < stlbEnd) //go up
	{
		//delete the up wall of the start cell
		labyrinth.cells[strStart][stlbStart] = deleteWall(labyrinth.cells[strStart][stlbStart], 2);
		//delete the down wall of the end cell
		labyrinth.cells[strEnd][stlbEnd] = deleteWall(labyrinth.cells[strEnd][stlbEnd], 0);
	}
}

//get all unvizited cells
void Labyrinth::getUnvizited()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (gone[i][j] == 0)
			{
				unvisited.push_back(Vec2(i, j));
			}
		}
	}
}

bool Labyrinth::isVisited(int str, int stlb)
{
	if (gone[str][stlb] == 1)
		return true;
	else
		return false;
}

//check if there is a wall between cells
bool Labyrinth::isWall(int rowStart, int colStart, int rowEnd, int colEnd)
{

	if (rowStart > rowEnd) //go left
	{
		log("wall1 is %d", labyrinth.cells[rowStart][colStart] & (1 << 3));
		log("wall2 is %d", labyrinth.cells[rowEnd][colEnd] & (1 << 1));

		if ((labyrinth.cells[rowStart][colStart] & (1 << 3)) == 0 && (labyrinth.cells[rowEnd][colEnd] & (1 << 1)) == 0)
			return false;
		else
			return true;
	}
	else if (rowStart < rowEnd) //go right
	{
		log("wall1 is %d", labyrinth.cells[rowStart][colStart] & (1 << 1));
		log("wall2 is %d", labyrinth.cells[rowEnd][colEnd] & (1 << 3));

		if ((labyrinth.cells[rowStart][colStart] & (1 << 1)) == 0 && (labyrinth.cells[rowEnd][colEnd] & (1 << 3)) == 0)
			return false;
		else
			return true;
	}
	else if (colStart > colEnd) //go down
	{
		log("wall1 is %d", labyrinth.cells[rowStart][colStart] & (1 << 0));
		log("wall2 is %d", labyrinth.cells[rowEnd][colEnd] & (1 << 2));

		if ((labyrinth.cells[rowStart][colStart] & (1 << 0)) == 0 && (labyrinth.cells[rowEnd][colEnd] & (1 << 2)) == 0)
			return false;
		else
			return true;
	}
	else if (colStart < colEnd) //go up
	{
		log("wall1 is %d", labyrinth.cells[rowStart][colStart] & (1 << 2));
		log("wall2 is %d", labyrinth.cells[rowEnd][colEnd] & (1 << 0));

		if ((labyrinth.cells[rowStart][colStart] & (1 << 2)) == 0 && (labyrinth.cells[rowEnd][colEnd] & (1 << 0)) == 0)
			return false;
		else
			return true;
	}
}
void Labyrinth::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();
}
