//
// Created by Weilai Xiang on 21/07/12.
//

#ifndef IMAGE_DESCRIPTOR_H
#define IMAGE_DESCRIPTOR_H

#include "Mathematics.h"

typedef std::vector<Complex> ComplexPoints;

ComplexPoints getEdgePoints();

ComplexPoints getFourierDescriptors(ComplexPoints &points);

ComplexPoints getIFourierRecon(ComplexPoints &desc, int M);

void plotPoints(const ComplexPoints &points, const char *title);

#endif //IMAGE_DESCRIPTOR_H
