#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <pthread.h>

struct ThreadData {
    long double* result;
    long double num_samples;
};

void* calculate_orthant(void *thread_args) {
    ThreadData* data = (ThreadData*)thread_args;
    long double* result = data->result;
    long double num_samples = data->num_samples;

    long double inside = 0;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<long double> distribution(0.0L, 1.0L);

    for (long double i = 0; i < num_samples; i++) {
        long double x_comp = distribution(generator);
        long double y_comp = distribution(generator);
        
        long double norm = sqrt(x_comp * x_comp + y_comp * y_comp);
        if (norm <= 1.0L) {
            inside++;
        }
    }
    *result = inside;

    pthread_exit(NULL);
}

int main() {
    int num_threads = 4;
    long double num_samples = 10000000.0L;
    std::vector<long double> flummoxes(num_threads);
    pthread_t threads[num_threads];
    std::vector<ThreadData> thread_data(num_threads);

    long double total_inside = 0.0L;

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].result = &flummoxes[i];
        thread_data[i].num_samples = num_samples;
        pthread_create(&threads[i], NULL, calculate_orthant, (void*)&thread_data[i]);
    }

    for (int j = 0; j < num_threads; j++) {
        pthread_join(threads[j], NULL); 
    }

    for (int k = 0; k < num_threads; k++) {
        total_inside += flummoxes[k];
    }

    long double pi = 4.0L * total_inside / (num_samples * num_threads);

    std::cout << "After " << num_samples << " iterations on each of " << num_threads << " threads, the value of pi = " << std::setprecision(20) << pi << std::endl;

    return 0;
}
