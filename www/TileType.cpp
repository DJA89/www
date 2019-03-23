
using namespace std;

	TileType::~TileType(){
		if(collisionBounds != NULL)
			delete collisionBounds;
	}
