#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "pipeline/pipeline.h"

int main() {
  Pipeline pipeline(2);
  // Define processing functions for each stage
  pipeline.AddStage(
      0,
      [](DataItem& item) {
        // Stage 1 processing logic
        int* data = static_cast<int*>(item.data);
        *data *= 2;
      },
      5, std::chrono::milliseconds(100));

  pipeline.AddStage(
      1,
      [](DataItem& item) {
        // Stage 2 processing logic
        // You can customize this function as needed
        // Cast item.data to the appropriate type and process it
        int* data = static_cast<int*>(item.data);
        if (data) {
          *data += 1;  // Increment the value
          std::cout << *data << std::endl;
        }
      },
      5, std::chrono::milliseconds(100));

  // Run the pipeline
  pipeline.Run();

  for (int i = 0; i < 100; i++) {
    DataItem item;
    item.data = new int(i);
    pipeline.PushData(item);
  }

  std::this_thread::sleep_for(std::chrono::seconds(2));
  // Finish the pipeline when data generation is complete
  pipeline.Finish();

  return 0;
}
