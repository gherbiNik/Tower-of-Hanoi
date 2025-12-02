#pragma once
#include "object.h"
#include "node.h"
#include <list>
#include "libConfig.h"


class ENG_API List : public Object {
public:
	List();
	~List();
	void pass(Node* node, glm::mat4 mat);
	void render(glm::mat4 viewMatrix);
	void render() override;
	void clear();

protected:
	struct Instance {
		Node* node;
		glm::mat4 nodeWorldMatrix;
	};


	std::list<Instance> instances;
};


