import matplotlib.pyplot as plt
import numpy as np
from numpy.fft import fft, ifft
import time



# sampling rate
sr = 100000
# sampling interval
ts = 1.0/sr
t = np.arange(0, 1, ts)

freq = 10
x = 3*np.sin(2*np.pi*freq*t)

freq = 200
x += np.sin(2*np.pi*freq*t)

freq = 700
x += 0.5 * np.sin(2*np.pi*freq*t)


start = time.time()
X = fft(x)/sr
end = time.time()
print("The time of execution of above program is :", end-start)
print("tamanho do x:", len(X))
N = len(X)
n = np.arange(N)
T = N/sr
freq = n/T


class CircularBuffer(object):
    def __init__(self, max_size=10):
        """Initialize the CircularBuffer with a max_size if set, otherwise
        max_size will elementsdefault to 10"""
        self.buffer = [None] * max_size
        self.head = 0
        self.tail = 0
        self.max_size = max_size

    def __str__(self):
        """Return a formatted string representation of this CircularBuffer."""
        items = ['{!r}'.format(item) for item in self.buffer]
        return '[' + ', '.join(items) + ']'

    def size(self):
        """Return the size of the CircularBuffer
        Runtime: O(1) Space: O(1)"""
        if self.tail >= self.head:
            return self.tail - self.head
        return self.max_size - self.head - self.tail

    def is_empty(self):
        """Return True if the head of the CircularBuffer is equal to the tail,
        otherwise return False
        Runtime: O(1) Space: O(1)"""
        return self.tail == self.head

    def is_full(self):
        """Return True if the tail of the CircularBuffer is one before the head,
        otherwise return False
        Runtime: O(1) Space: O(1)"""
        return self.tail == (self.head-1) % self.max_size

    def enqueue(self, item):
        """Insert an item at the back of the CircularBuffer
        Runtime: O(1) Space: O(1)"""
        if self.is_full():
            raise OverflowError(
                "CircularBuffer is full, unable to enqueue item")
        self.buffer[self.tail] = item
        self.tail = (self.tail + 1) % self.max_size

    def front(self):
        """Return the item at the front of the CircularBuffer
        Runtime: O(1) Space: O(1)"""
        return self.buffer[self.head]

    def dequeue(self):
        """Return the item at the front of the Circular Buffer and remove it
        Runtime: O(1) Space: O(1)"""
        if self.is_empty():
            raise IndexError("CircularBuffer is empty, unable to dequeue")
        item = self.buffer[self.head]
        self.buffer[self.head] = None
        self.head = (self.head + 1) % self.max_size
        return item


# Examples
cb = CircularBuffer(5)
print("Empty: {}".format(cb.is_empty()))
print("Full: {}".format(cb.is_full()))
print(str(cb))
cb.enqueue("one")
cb.enqueue("two")
cb.enqueue("three")
cb.enqueue("four")
print(str(cb))
print(cb.dequeue())
print(cb.dequeue())
print(str(cb))
cb.enqueue("five")
cb.enqueue("six")
print(str(cb))
print("Full: {}".format(cb.is_full()))

#plt.figure(figsize = (12, 6))
# plt.subplot(121)

# plt.stem(freq, np.abs(X), 'b', \
# markerfmt=" ", basefmt="-b")
#plt.xlabel('Freq (Hz)')
#plt.ylabel('FFT Amplitude |X(freq)|')
#plt.xlim(0, 1000)

# plt.subplot(122)
#plt.plot(t, ifft(X), 'r')
#plt.xlabel('Time (s)')
# plt.ylabel('Amplitude')
# plt.tight_layout()
# plt.show()


