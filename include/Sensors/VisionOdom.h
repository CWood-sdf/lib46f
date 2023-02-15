#include "Odometry/EPA_Tracker.h"
#include "Sensors/Vision/FieldObject.h"
#include "Sensors/Vision/ObjectMap.h"
// class Angle {
// public:
//   enum class Type {
//     Deg,
//     Rad,
//     Rev
//   };
// private:
//   double value = 0.0;//In rad
//   Type type = Type::Rad;
//   static inline map<Type, double> toRad = map<Type, double>();
//   static void init();
// public:
//   Angle(double v, Type t);
//   Angle(double v);
//   double as(Type t);
//   double as(vex::rotationUnits t);
//   Angle& changeType(Type t);
//   Angle& from(double v, Type t);
//   operator double();
// };
// Angle operator "" _deg(long double);
// Angle operator "" _rev(long double);
// Angle operator "" _rad(long double);
// extern GPS_Share share;
// class VisionOdom {
// protected:
//   vision* sensor;
//   double mountHeight, mountAngle, mountRotation;
//   PVector relPos;
//   // static inline BasicWheelController* wc;
//   static constexpr double
//     screenWidth = 310,
//     screenHeight = 210,
//     widthAngle = 56 * DEG_TO_RAD,
//     heightAngle = 46 * DEG_TO_RAD,
//     backDist = 0.5 * screenWidth / _tan(widthAngle / 2.0);
//   PVector estimateSensorRelativePos(int pixelLeftX, int pixelBottomY, int pixelRightX, int pixelTopY, PVector bottomPoint, PVector topPoint, PVector fixedPoint);
//   PVector estimateSensorRelativePos(vision::signature* sig, PVector bottomPoint, PVector topPoint, PVector fixedPoint);
//   PVector estimateRelativePos(int pixelLeft, int pixelRight, double radius, double objHeight);
//   Angle getAngleY(int pixelY);
//   Angle getAngleX(int pixelX);
// public:

//   VisionOdom(vision& sensor, PVector relPos, double height, double mAngle, double mRotation);
//   PVector estimateRelativePos(vision::signature* sig, double radius, double objHeight);
//   // template<class Sig>
//   // PVector estimatePos(Sig& sig){
//   //   sensor->takeSnapshot(sig);
//   //   return estimatePos(sensor->largestObject);
//   // }
//   // template<class Sig>
//   // vector<PVector> estimateAllPos(Sig& sig){
//   //   sensor->takeSnapshot(sig);
//   //   vector<PVector> ret;
//   //   for(int i = 0; i < sensor->objects.getLength(); i++){
//   //     if(sensor->objects[i].exists){
//   //       ret.push_back(estimatePos(sensor->objects[i]));
//   //     }
//   //   }
//   //   return ret;
//   // }
//   // template<class Sig>
//   // PVector closest(Sig& sig){
//   //   sensor->takeSnapshot(sig);
//   //   vector<PVector> arr;
//   //   for(int i = 0; i < sensor->objects.getLength(); i++){
//   //     if(sensor->objects[i].exists){
//   //       //auto pos = estimatePos(sensor->objects[i]);
//   //       arr.push_back(estimatePos(sensor->objects[i]));
//   //     }
//   //   }
//   //   PVector c;
//   //   double dist = 100000;
//   //   for(auto i : arr){
//   //     if(i.dist2D(share.position()) < dist){
//   //       dist = i.dist2D(share.position());
//   //       c = i;
//   //     }
//   //   }
//   //   return c;
//   // }
//   // PVector getEstimatePos(vision::object& object, CircFieldObject& obj){
//   //   auto topLeft = estimateRelPos(object.originX, object.originY);
//   //   auto btmRight = estimateRelPos(object.originX + object.width, object.originY + object.height);
//   //   PVector ret = Refine::getRelPosLocation(topLeft.first, btmRight.first, mountHeight, topLeft.second, btmRight.second, obj);
//   //   return ret;
//   // }
//   safearray<PVector, VISION_MAX_OBJECTS> fromArr(safearray<vision::object, VISION_MAX_OBJECTS>& arr);
//   safearray<PVector, VISION_MAX_OBJECTS> fromMap(ObjectMap& objects);
// };

// class VisionBasicOdometry : private VisionOdom, public BasicConfirmationOdom {
//   PVector currentEstimate;
//   vector<tuple<vision::signature*, PVector, PVector, PVector>> fixedPoints;
//   PVector posOffCenter = PVector(0, 0, 0);
//   int currentQuality = 0;
//   double variance() override {
//     return 16.0;
//   }
//   void updateEstimate() override {
//     //Update the current estimate from the fixed points, if the estimate is outside the 12' by 12' field, set quality = 0
//     for (auto& tuple : fixedPoints) {
//       auto& sig = get<0>(tuple);
//       auto& bottomPoint = get<1>(tuple);
//       auto& topPoint = get<2>(tuple);
//       auto& fixedPoint = get<3>(tuple);
//       PVector estimate = estimateSensorRelativePos(sig, bottomPoint, topPoint, fixedPoint);
//       currentEstimate = estimate;
//       PVector posOffCopy = posOffCenter;
//       posOffCopy.rotateXY(-share.heading());
//       currentEstimate += posOffCopy;
//       if (currentEstimate.dist2D({0, 0}) >= 12.0 * 6.0) {
//         currentQuality = 0;
//       }
//       else {
//         currentQuality = 100;
//         break;
//       }
//     }
//   }
//   double xPosition(distanceUnits) override {
//     return currentEstimate.x;
//   }
//   double yPosition(distanceUnits) override {
//     return currentEstimate.y;
//   }
//   int32_t quality() override {
//     return currentQuality;
//   }
//   bool installed() override {
//     return sensor->installed();

//   }
// };
