#include "mvpprovider.h"

MVPProvider::MVPProvider()
{

}

QMatrix4x4 MVPProvider::getMVPMatrix()
{
    return mvp_matrix;
}
