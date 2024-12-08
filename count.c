#include <stdio.h>
#include <time.h>

int main() {
    // Number of iterations
    long long int iterations = 1000000000;
    
    // Start time
    clock_t start_time = clock();
    
    // Perform iterations
    for (long long int i = 0; i < iterations; i++) {
        printf("%lld\n", i);
        // No operation, just iterating
    }
    
    // End time
    clock_t end_time = clock();
    
    // Calculate total time in seconds
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    // Print total time
    printf("Total time taken: %f seconds\n", total_time);
    
    return 0;
}
