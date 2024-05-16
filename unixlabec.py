import threading
import random
import time
import queue
import sys

# Buffer class implementing FIFO
class Buffer:
    def __init__(self, size):
        self.buffer = queue.Queue(size)
        self.size = size
        self.lock = threading.Lock()
    
    def add_item(self, item, producer_id):
        with self.lock:
            self.buffer.put(item)
            self.print_buffer(f"item {item} added by Producer {producer_id}")

    def remove_item(self, consumer_id):
        with self.lock:
            item = self.buffer.get()
            self.print_buffer(f"item {item} taken by Consumer {consumer_id}")
            return item
    
    def print_buffer(self, action):
        buffer_state = list(self.buffer.queue)
        buffer_state += [0] * (self.size - len(buffer_state))
        print(f"{action}: buffer = {buffer_state}")

# Producer thread
def producer(buffer, producer_id, terminate_event):
    while not terminate_event.is_set():
        time.sleep(random.uniform(0.5, 2.0))  
        item = random.randint(0, 100)
        buffer.add_item(item, producer_id)

# Consumer thread
def consumer(buffer, consumer_id, terminate_event):
    while not terminate_event.is_set():
        time.sleep(random.uniform(0.5, 2.0))  
        buffer.remove_item(consumer_id)

def main():
    if len(sys.argv) != 4:
        print("Usage: python3 unixlabec.py <sleep_time> <num_producers> <num_consumers>")
        return
    
    sleep_time = int(sys.argv[1])
    num_producers = int(sys.argv[2])
    num_consumers = int(sys.argv[3])

    buffer_size = 5  # Increased buffer size
    buffer = Buffer(buffer_size)
    
    terminate_event = threading.Event()
    
    # Create and start producer threads
    producer_threads = []
    for i in range(num_producers):
        t = threading.Thread(target=producer, args=(buffer, i + 1, terminate_event))
        producer_threads.append(t)
        t.start()
    
    # Create and start consumer threads
    consumer_threads = []
    for i in range(num_consumers):
        t = threading.Thread(target=consumer, args=(buffer, i + 1, terminate_event))
        consumer_threads.append(t)
        t.start()
    
    # Run the simulation for the given sleep time
    time.sleep(sleep_time)
    
    # Signal threads to terminate
    terminate_event.set()
    
    # Wait for all threads to finish
    for t in producer_threads + consumer_threads:
        t.join()

    print("Simulation complete.")

if __name__ == "__main__":
    main()
