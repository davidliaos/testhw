#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Data structure for Job
struct Job {
    int size;
    string status;
    int partition;
};

// Function to display the memory allocation
void displayAllocation(const vector<int>& partitions, const vector<Job>& jobs) {
    cout << "\nInitial Memory Allocation:\n";
    for (size_t i = 0; i < partitions.size(); ++i) {
        cout << "Partition " << i + 1 << ": " << partitions[i] << " KB\n";
    }
    cout << "\nJobs Allocation:\n";
    for (size_t i = 0; i < jobs.size(); ++i) {
        cout << "Job " << i + 1 << " (Size: " << jobs[i].size << " KB) - Status: " << jobs[i].status;
        if (jobs[i].status == "Run") {
            cout << " - Partition: " << jobs[i].partition + 1;
        }
        cout << endl;
    }
}

// Function to implement First-fit memory allocation
void firstFitMemoryAllocation(vector<int>& partitions, vector<Job>& jobs) {
    for (size_t i = 0; i < jobs.size(); ++i) {
        for (size_t j = 0; j < partitions.size(); ++j) {
            if (jobs[i].size <= partitions[j]) {
                jobs[i].status = "Run";
                jobs[i].partition = j;
                partitions[j] -= jobs[i].size;
                break;
            }
        }
        if (jobs[i].status != "Run") {
            jobs[i].status = "Wait";
        }
    }
}

int main() {
    int numPartitions, numJobs;
    
    cout << "Enter the number of partitions: ";
    cin >> numPartitions;
    
    vector<int> partitions(numPartitions);
    cout << "Enter the sizes of partitions: ";
    for (int i = 0; i < numPartitions; ++i) {
        cin >> partitions[i];
    }
    
    cout << "Enter the number of jobs: ";
    cin >> numJobs;
    
    vector<Job> jobs(numJobs);
    cout << "Enter the sizes of jobs: ";
    for (int i = 0; i < numJobs; ++i) {
        cin >> jobs[i].size;
        jobs[i].status = "Wait";
        jobs[i].partition = -1;
    }
    
    firstFitMemoryAllocation(partitions, jobs);
    displayAllocation(partitions, jobs);
    
    // Display memory waste
    int memoryWaste = 0;
    for (size_t i = 0; i < partitions.size(); ++i) {
        memoryWaste += partitions[i];
    }
    cout << "\nTotal Memory Waste: " << memoryWaste << " KB\n";
    
    return 0;
}
