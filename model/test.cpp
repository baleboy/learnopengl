#include <vector>
#include <string>

using namespace std;

struct Texture {
    unsigned int id;
    string type;
    string path;
};

vector<Texture> getTextures()
{
    vector<Texture> textures;
    Texture texture;
    textures.push_back(texture);
    return textures;
}

int main()
{
    if (1) {
        vector<Texture> textures = getTextures();
    }
    return 0;    
}
