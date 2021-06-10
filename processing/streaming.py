# Modified from imutils python library: https://github.com/jrosebr1/imutils
import cv2
import time
import timebudget

from collections import namedtuple
from threading import Thread
from queue import Queue

Frame = namedtuple('Frame', ['image', 'position_ms'])

class FileVideoStream:
  def __init__(self, path, queue_size=128):
    self.capture = cv2.VideoCapture(path)
    self.fps = self.capture.get(cv2.CAP_PROP_FPS)
    frame_count = self.capture.get(cv2.CAP_PROP_FRAME_COUNT)
    self.duration_ms = int((frame_count / self.fps) * 1000)
    self.width = int(self.capture.get(cv2.CAP_PROP_FRAME_WIDTH))
    self.height = int(self.capture.get(cv2.CAP_PROP_FRAME_HEIGHT))
    self.shape = (self.width, self.height)

    self.stopped = False
    self.queue = Queue(maxsize=queue_size)
    self.thread = Thread(target=self.update, args=())
    self.thread.daemon = True

  def start(self):
    # start a thread to read frames from the file video stream
    self.thread.start()

  def update(self):
    # keep looping infinitely
    while True:
      # if the thread indicator variable is set, stop the
      # thread
      if self.stopped:
        break

      # otherwise, ensure the queue has room in it
      if not self.queue.full():
        # read the next frame from the file
        (grabbed, frame) = self.capture.read()

        # if the `grabbed` boolean is `False`, then we have
        # reached the end of the video file
        if not grabbed:
          self.stopped = True

        # add the frame to the queue
        position = self.capture.get(cv2.CAP_PROP_POS_MSEC)
        frame_info = Frame(frame, position)
        self.queue.put(frame_info)
      else:
        time.sleep(0.1)  # Rest for 10ms, we have a full queue

    self.stream.release()

  @timebudget.timebudget
  def read(self):
    # return next frame in the queue
    return self.queue.get()

  @property
  def running(self):
    return self.more() or not self.stopped

  def more(self):
    # return True if there are still frames in the queue. If stream is not stopped, try to wait a moment
    tries = 0
    while self.queue.qsize() == 0 and not self.stopped and tries < 5:
      time.sleep(0.1)
      tries += 1

    return self.queue.qsize() > 0

  def stop(self):
    # indicate that the thread should be stopped
    self.stopped = True
    # wait until stream resources are released (producer thread might be still grabbing frame)
    self.thread.join()