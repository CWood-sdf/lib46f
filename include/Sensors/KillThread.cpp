// #include "Sensors/KillThread.h"
// KillThread::KillThread(void(*callback)(void) ) : t(new thread(callback)) {
//   instances.push_back(t);
// }
// KillThread::KillThread(thread&& t) : t(&t){
//   instances.push_back(this->t);
// }
// KillThread& KillThread::operator=(thread&& t){
//   this->t = &t;
//   instances.push_back(&t);
//   return *this;
// }
// void KillThread::killAll(){
//   cout << "KAT " << instances.size() << endl;
//   for(auto i : instances){
//     // i->join();
//     i->interrupt();
//     // i->detach();
//   }
//   instances.clear();
// }
