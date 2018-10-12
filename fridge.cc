#include "Fridge.h"
 
class Fridge::FridgeImpl
{
public:
   void coolDown()
   {
      printf("help");
   }
private:
	int engine_ = 0;
};
 
Fridge::Fridge() : pImpl_(new FridgeImpl) {}
 
Fridge::~Fridge() = default;

void Fridge::coolDown()
{
	pImpl_ ->coolDown();
}


int main()
{
	Fridge greg;
	greg.coolDown();
}