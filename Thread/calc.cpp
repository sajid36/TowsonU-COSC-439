#include <iostream>
#include <pthread.h>

void* do_sum(void* arg)
{
    int* nums = (int*)arg;
    int* result = new int(nums[0] + nums[1]);
    pthread_exit(result);
}

void* do_sub(void* arg)
{
    int* nums = (int*)arg;
    int* result = new int(nums[0] - nums[1]);
    pthread_exit(result);
}

void* do_mul(void* arg)
{
    int* nums = (int*)arg;
    int* result = new int(nums[0] * nums[1]);
    pthread_exit(result);
}

void* do_div(void* arg)
{
    int* nums = (int*)arg;
    int* result = new int(0);

    if (nums[1] != 0)
        *result = nums[0] / nums[1];

    pthread_exit(result);
}

int main()
{
    pthread_t t1, t2, t3, t4;

    int values[2] = {20, 5};   // change numbers here

    pthread_create(&t1, NULL, do_sum, (void*)values);
    pthread_create(&t2, NULL, do_sub, (void*)values);
    pthread_create(&t3, NULL, do_mul, (void*)values);
    pthread_create(&t4, NULL, do_div, (void*)values);

    void* r1;
    void* r2;
    void* r3;
    void* r4;

    pthread_join(t1, &r1);
    pthread_join(t2, &r2);
    pthread_join(t3, &r3);
    pthread_join(t4, &r4);

    int sum = *(int*)r1;
    int sub = *(int*)r2;
    int mul = *(int*)r3;
    int divv = *(int*)r4;

    delete (int*)r1;
    delete (int*)r2;
    delete (int*)r3;
    delete (int*)r4;

    std::cout << "a = " << values[0] << ", b = " << values[1] << std::endl;
    std::cout << "Sum = " << sum << std::endl;
    std::cout << "Sub = " << sub << std::endl;
    std::cout << "Mul = " << mul << std::endl;

    if (values[1] == 0)
        std::cout << "Div = undefined (division by zero)" << std::endl;
    else
        std::cout << "Div = " << divv << std::endl;

    return 0;
}
