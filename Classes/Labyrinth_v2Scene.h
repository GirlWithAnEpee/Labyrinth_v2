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

#ifndef __LABYRINTH_SCENE_H__
#define __LABYRINTH_SCENE_H__

#include "cocos2d.h"
#include <time.h>

class Labyrinth : public cocos2d::Scene
{
public:
	int strStart, stlbStart;
	int length = 0;
	std::vector<cocos2d::Vec2> way;
	std::vector<cocos2d::Vec2> unvisited;
	std::vector<cocos2d::Vec2> borders;

	static const int SIZE = 10;
	const int CELL_SIZE = 50;
	const int NEIBS = 4;
	cocos2d::Vec2 origin;
	struct SLabyrinth
	{
		int cells[SIZE][SIZE];
	} labyrinth;

	int gone[SIZE][SIZE];

	static cocos2d::Scene* createScene();

	cocos2d::DrawNode* labyrinthDN;
	cocos2d::DrawNode* startDN;

	virtual bool init();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void createCage(); //generate labyrinth
	void drawCage(); //draw labyrinth
	void findWay(int x, int y); //find the way from start point
	void goThroughWalls(int strStart, int stlbStart, int strEnd, int stlbEnd); //go from current cell to the next cell
	int deleteWall(int cell, int pos); //delete current wall of current cell
	std::vector<cocos2d::Vec2> findNeibs(int strStart, int stlbStart); //returns the next point for the current point
	void getUnvizited(); //get all unvizited cells
	bool isWall(int rowStart, int colStart, int rowEnd, int colEnd); //check if there is a wall between cells

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Labyrinth);
};

#endif // __LABYRINTH_SCENE_H_
