//
// Created by giuseppe on 18/12/16.
//

#include "Drawable.h"
#include "../Color.h"

Drawable::Drawable() {
    this->attrColor = Color::Reset;
    this->foreColor = Color::Default;
    this->backColor = Color::Default;
}
