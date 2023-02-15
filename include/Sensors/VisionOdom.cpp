// #include "Sensors/VisionOdom.h"
// Angle operator "" _deg(long double v){
//     return Angle((double) v, Angle::Type::Deg);
// }
// Angle operator "" _rev(long double v){
//     return Angle((double) v, Angle::Type::Rev);
// }
// Angle operator "" _rad(long double v){
//     return Angle((double) v, Angle::Type::Rad);
// }
// void Angle::init(){
//     static bool done = false;
//     if(!done){
//         done = true;
//         toRad[Type::Rad] = 1;
//         toRad[Type::Deg] = M_PI / 180;
//         toRad[Type::Rev] = 2.0 * M_PI;
//     }
// }
// Angle::Angle(double v, Type t){
// 	init();
// 	from(v, t);
// }
// Angle::Angle(double v){
// 	init();
// 	from(v, Type::Rad);
// }
// double Angle::as(Type t){
// 	if(t == Type::Rad){
// 		return value * toRad[type];
// 	}
// 	return value * toRad[type] / toRad[t];
// }
// double Angle::as(vex::rotationUnits t){
// 	switch (t)
// 	{
// 	case rotationUnits::deg:
// 		return as(Type::Deg);
// 		break;
// 	case rotationUnits::raw:
// 		return as(Type::Rad);
// 		break;
// 	case rotationUnits::rev:
// 		return as(Type::Rev);
// 		break;
// 	}
// }
// Angle& Angle::changeType(Type t){
// 	value = as(t);
// 	type = t;
// 	return *this;
// }
// Angle& Angle::from(double v, Type t){
// 	value = v;
// 	type = t;
// 	return *this;
// }
// Angle::operator double(){
// 	return as(Type::Rad);
// }

// PVector VisionOdom::estimateSensorRelativePos(int pixelLeftX, int pixelBottomY, int pixelRightX, int pixelTopY, PVector bottomPoint, PVector topPoint, PVector fixedPoint) {

// 	if (pixelLeftX == 0 || pixelTopY == 0 || pixelBottomY >= screenHeight || pixelRightX >= screenWidth) {
// 		return PVector(1e10, 1e10);
// 	}

// 	//The angle to the top and bottom points
// 	Angle angleYBottom = getAngleY(pixelBottomY) - mountAngle;
// 	Angle angleYTop = getAngleY(pixelTopY) - mountAngle;
// 	Angle angleXBottom = getAngleX(pixelLeftX);
// 	Angle angleXTop = getAngleX(pixelRightX);
// 	Angle angleXAvg = (angleXBottom + angleXTop) / 2.0 - share.heading() - mountRotation;

// 	double topDiff = topPoint.z - mountHeight;
// 	double bottomDiff = bottomPoint.z - mountHeight;

// 	double distFromBottom = bottomDiff / tan(angleYBottom);
// 	double distFromTop = topDiff / tan(angleYTop);
// 	bottomPoint = PVector(0, bottomPoint.y).rotateXY(-angleXAvg);
// 	topPoint = PVector(0, topPoint.y).rotateXY(-angleXAvg);
// 	PVector bottomEst = PVector(0, distFromBottom).rotateXY(-angleXAvg);
// 	bottomEst.add(bottomPoint);
// 	PVector topEst = PVector(0, distFromTop).rotateXY(-angleXAvg);
// 	topEst.add(topPoint);
// 	PVector ret = (bottomEst + topEst) / 2.0;
// 	ret.z += mountHeight;
// 	fixedPoint.z = 0;
// 	ret.add(fixedPoint);
// 	return ret;
// }
// PVector VisionOdom::estimateSensorRelativePos(vision::signature* sig, PVector bottomPoint, PVector topPoint, PVector fixedPoint) {
// 	sensor->takeSnapshot(*sig);
// 	auto& largestObj = sensor->largestObject;
// 	if (!largestObj.exists) {
// 		//Return an absolutely large estimate if no object is found
// 		return { 1e10, 1e10 };
// 	}
// 	return estimateSensorRelativePos(
// 		largestObj.originX,
// 		largestObj.originY + largestObj.height,
// 		largestObj.originX + largestObj.width,
// 		largestObj.originY,
// 		bottomPoint,
// 		topPoint,
// 		fixedPoint);
// }
// PVector VisionOdom::estimateRelativePos(int pixelLeft, int pixelRight, double radius, double objHeight){
// 	Angle angleXLeft = getAngleX(pixelLeft);
// 	Angle angleXRight = getAngleX(pixelRight);
// 	cout << "AngleXLeft: " << angleXLeft.as(Angle::Type::Deg) << endl;
// 	cout << "AngleXRight: " << angleXRight.as(Angle::Type::Deg) << endl;
// 	//The internal angle is 0.5 times the difference
// 	Angle internalAngle = (angleXLeft - angleXRight) / 2.0;
// 	cout << "Internal Angle: " << internalAngle.as(Angle::Type::Deg) << endl;
// 	//The local offset is the avg
// 	Angle localOffset = (angleXLeft + angleXRight) / 2.0;
// 	cout << "Local Offset: " << localOffset.as(Angle::Type::Deg) << endl;
// 	//The distance to the object is the radius * the csc of internalAngle
// 	//The distance between the camera and the center point
// 	double dist = radius / sin(internalAngle);
// 	cout << "Dist: " << dist << endl;
// 	double heightDiff = mountHeight - objHeight;
// 	cout << "Height Diff: " << heightDiff << endl;
// 	//The angle to rotateYZ down by is the 90 - acos(heightDiff / dist)
// 	Angle angleYZ = 90.0_deg - Angle(acos(heightDiff / dist), Angle::Type::Rad);
// 	cout << "Angle YZ: " << angleYZ.as(Angle::Type::Deg) << endl;
// 	PVector ret = PVector(0, dist, 0);
// 	ret.rotateYZ(-angleYZ.as(Angle::Type::Deg));
// 	ret.rotateXY(localOffset.as(Angle::Type::Deg));
// 	ret.z += mountHeight;
// 	//ret.pt.z = 0;
// 	return ret;
// }
// Angle VisionOdom::getAngleY(int pixelY){
// 	//backDist
// 	cout << "BackDist: " << backDist << endl;
// 	//screenHeight
// 	cout << "ScreenHeight: " << screenHeight << endl;
// 	//pixelY
// 	cout << "PixelY: " << pixelY << endl;
// 	//mountAngle
// 	cout << "MountAngle: " << mountAngle << endl;
// 	//atan((screenHeight / 2.0 - pixelY) / backDist)
// 	cout << "Atan: " << atan((screenHeight / 2.0 - pixelY) / backDist) << endl;
// 	return Angle(-atan(-(screenHeight / 2.0 - pixelY) / backDist) + mountAngle * DEG_TO_RAD, Angle::Type::Rad);
// }
// Angle VisionOdom::getAngleX(int pixelX){
// 	//backDist
// 	cout << "BackDist: " << backDist << endl;
// 	//screenWidth
// 	cout << "ScreenWidth: " << screenWidth << endl;
// 	//pixelX
// 	cout << "PixelX: " << pixelX << endl;
// 	//mountRotation
// 	cout << "MountRotation: " << mountRotation << endl;
// 	//atan((screenWidth / 2.0 - pixelX) / backDist)
// 	cout << "Atan: " << atan((screenWidth / 2.0 - pixelX) / backDist) << endl;
// 	return Angle( atan(-(screenHeight / 2.0 - pixelX) / backDist) + mountRotation * DEG_TO_RAD, Angle::Type::Rad);
// }

// VisionOdom::VisionOdom(vision& sensor, PVector relPos, double height, double mAngle, double mRotation){
// 	this->relPos = relPos;
// 	this->sensor = &sensor;
// 	mountHeight = height;
// 	mountAngle = mAngle * DEG_TO_RAD;
// 	mountRotation = mRotation;
// }
// PVector VisionOdom::estimateRelativePos(vision::signature* sig, double radius, double objHeight){
// 	sensor->takeSnapshot(*sig);
// 	auto& largestObj = sensor->largestObject;
// 	if (!largestObj.exists) {
// 		//Return an absolutely large estimate if no object is found
// 		return { 400, 400 };
// 	}
// 	return estimateRelativePos(
// 		largestObj.originX,
// 		largestObj.originX + largestObj.width,
// 		radius,
// 		objHeight);
// }
