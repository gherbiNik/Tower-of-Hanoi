#pragma once
#include "object.h"
#include "node.h"
#include <vector>


class List : public Object {
public:
	List();
	~List();
	void pass(Node* node, glm::mat4 mat);
	void render();
	void clear();

private:
	struct Instance {
		Node* node;
		glm::mat4 nodeWorldMatrix;
	};

	std::vector<Instance> instances;
};


