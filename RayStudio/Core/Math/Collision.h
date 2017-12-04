#pragma once

class Collision
{
public:
	Collision();
	~Collision();



};



class BoXCollision : public Collision
{
public:
	float XMin, XMaX;


};


class SphereCollision : public Collision
{
public:

};

class CapsuleCollision : public Collision
{

};