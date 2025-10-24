#include "carPhysics.h"

#include <cmath>

#define MAX_SPEED 20.0
#define ACCELERATION 5.0
#define ANGLE_ROTATION 4

CarPhyisics::CarPhysics(b2World& world, float x, float y):
	is_colliding(false) {
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		body = world.CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(1.0f, 2.0f);
		body->CreateFixture(&shape, 1.0f);

		body->setUserData(this);
	}

void CarPhysics::accelerate(){
	b2Vec2 direction(cos(body->GetAngle()), sin(body->GetAngle()));
	body->ApplyForceToCenter(ACCELERATION * direction, true);

	b2Vec2 velocity = body->GetLinearVelocity();
	if (velocity.Length() > MAX_SPEED){
		velocity.Normalize();
		body->SetLinearVelocity(velocity * MAX_SPEED);
	}
}

void CarPhysics::brake(){
	//tmb se puede frenar usando la velocidad lineal * 0.9
	b2Vec2 direction(cos(body->GetAngle()), sin(body->GetAngle()));
	body->ApplyForceToCenter((-ACCELERATION) * direction, true);
}

void CarPhysics::turn_left(){
	//tmb se puede usar el metodo ApplyTorque o con SetAngularVelocity
	float rad_rotation = ANGLE_ROTATION * (b2_pi / 180.0f);
	body->SetTransform(body->GetPosition(), body->GetAngle() - rad_rotation);
}

void CarPhysics::turn_right(){
	float rad_rotation = ANGLE_ROTATION * (b2_pi / 180.0f);
	body->SetTransform(body->GetPosition(), body->GetAngle() + rad_rotation);
}

float CarPhysics::get_x_position(){
	return body->GetPosition().x;
}

float CarPhysics::get_y_position(){
	return body->GetPosition().y;
}

float CarPhysics::get_angle(){
	float rad_angle = body->GetAngle();
	return (rad_angle * (180.0f / b2_pi));
}

float CarPhysics::get_speed(){
	return body->GetLinearVelocity().Length();
}
