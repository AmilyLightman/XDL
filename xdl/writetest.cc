#include "util/Buffer.hh"
#include "xdl/std.hh"
#include <iostream> 
using namespace std;

#if defined (__i386__)
static __inline__ unsigned long long GetCycleCount(void)
{
        unsigned long long int x;
        __asm__ volatile("rdtsc":"=A"(x));
        return x;
}
#elif defined (__x86_64__)
static __inline__ unsigned long long GetCycleCount(void)
{
        unsigned hi,lo;
        __asm__ volatile("rdtsc":"=a"(lo),"=d"(hi));
        return ((unsigned long long)lo)|(((unsigned long long)hi)<<32);
}
#endif

void timeTest(User student, Buffer& buf)
{
  unsigned long t1,t2;
        t1 = (unsigned long)GetCycleCount();
	student.writeMeta(buf);
	for(int i = 0; i < 100; i++ )
	  student.write(buf);
	t2 = (unsigned long)GetCycleCount();
        printf("Use Time:%f\n",(t2 - t1)*1.0/ADJ_FREQUENCY/1000000);
 
}

int main() {
  Buffer buf("teststudent.bin", 1000000);
  // List<User> students;
  
  UserId userid(223456);
  Address address("1313 Mockingbird Lanee");
  Phone phone(7123351567);
  Email email("wcao9@stevens.edu");
  User student(userid,  "Wanying", "Cao", address, phone, email);
  // students.add(student);
  //  students.add(223456, "Wanying", "Cao", "1313 Mockingbird Lanee", 7123351567, "wcao9@stevens.edu");
  timeTest(student, buf);

}
