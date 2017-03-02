#pragma once

#ifndef LSYSTEM_H
#define LSYSTEM_H
#include <vector> 
#include <map>
#include <list>
#include <string>

using namespace std;

class LSystem
{
private:
	map<int, list<int> > rules;
	list<int>* base_path;
	int prevIteration;
public:
	LSystem(const map<int, list<int> >& _rules, const list<int>& _base_path, const map<int, pair<int, int> >& _nodeOperation)
	{
		rules = _rules;
		base_path = new list<int>(_base_path);
		path = new list<int>(_base_path);
		nodeOperation = _nodeOperation;
		prevIteration = 0;
	}
	list<int>* path;
	map<int, pair<int, int> > nodeOperation;
	void generatePath(int iteration);
	void setForwardLength(int x);
};

void LSystem::generatePath(int iteration)
{
	if (prevIteration == iteration) return;
	prevIteration = iteration;
	if (path) delete path;
	path = new list<int>(*base_path);

	for (int i = 0; i < iteration; ++i)
	{
		list<int>* tmp = path;
		path = new list<int>;
		for (std::list<int>::iterator it = tmp->begin(); it != tmp->end(); it++)
		{
			if (rules.find(*it) != rules.end())
			{
				path->insert(path->end(), rules[*it].begin(), rules[*it].end());
			}
			else
			{
				path->insert(path->end(), *it);
			}
		}
		delete tmp;
	}

	//for (auto x : *path)
	//{
	//	printf("%d ", x);
	//}
	//printf("\n");
	
}
void LSystem::setForwardLength(int x)
{
	nodeOperation[0] = make_pair(0, x);
}
#endif