#pragma once

class FHealthComponent
{
public:
    explicit FHealthComponent(const float Health)
        : Health(Health)
    {
    }

    void TakeDamage();

private:
    float Health;
    
};

