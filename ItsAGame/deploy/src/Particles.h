#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

enum ParticleType { NORMAL_PARTICLE, AMMO_PARTICLE, SHRAPNEL_PARTICLE };

struct Particle
{
	int type;
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Vector2f acc;

	bool active = false;

	bool collidable;
	int collisionRadius;
	sf::Vector2f collisionBox;
	
	float timeToLive = 0.0f;
	float activeTime = 0.0f;
	class GameObject *owner = nullptr;
};

class ParticleSystem
{
public:
	ParticleSystem(int MaxParticles = 10000)
	{
		particles = new Particle[10000];
		count = 0;
		maxParticles = MaxParticles;
	}
	~ParticleSystem() {
		if (particles)
			delete[] particles;
	};

	void add(int type, float timeToLive, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f acceleration, 
			  bool collidable, int colRadius, sf::Vector2f collisionBox, class GameObject *ownerObj)
	{
		if (count < maxParticles-1)
		{
			particles[count].type = type;
			particles[count].pos  = position;
			particles[count].vel = velocity;
			particles[count].acc = acceleration;
			particles[count].collidable = collidable;
			particles[count].collisionRadius = colRadius;
			particles[count].collisionBox = collisionBox;
			particles[count].active = true;
			particles[count].timeToLive = timeToLive;
			particles[count].activeTime = 0.0f;
			particles[count++].owner = ownerObj;
		}
	}

	void moveToEnd(Particle *particleSlot)
	{
		if (count > 0)
		{
			particleSlot = &particles[count - 1];
			count--;
		}
	}

	void update(float dT)
	{
		for (int c = 0; c < count; c++)
		{
			Particle& particle = particles[c];

			if (particle.timeToLive > 0)
			{
				particle.activeTime += dT;
				if (particle.activeTime > particle.timeToLive)
				{
					particle.active = false;
					moveToEnd(&particles[c]);
					continue;
				}
			}

			switch (particle.type)
			{
			case NORMAL_PARTICLE:
				break;

			default:
				break;
			}
		}
	}

private:
	Particle *particles = nullptr;
	int count = 0;
	int maxParticles;
};

