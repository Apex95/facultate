"""
This module represents a device.

Computer Systems Architecture Course
Assignment 1
March 2016
"""

from threading import Thread, Lock, Event, Condition


class Barrier(object):
    """
    Class that represents a reusable barrier.
    """
    def __init__(self, num_threads):
        """
        Constructor.

        @type num_threads: Integer
        @param num_threads: Number of threads
        """
        self.num_threads = num_threads
        self.count_threads = self.num_threads
        self.cond = Condition()

    def wait(self):
        """
        Waits for "num_threads" threads to reach the barrier.
        """
        self.cond.acquire()
        self.count_threads -= 1
        if self.count_threads == 0:
            self.cond.notify_all()
            self.count_threads = self.num_threads
        else:
            self.cond.wait()
        self.cond.release()

class Device(object):
    """
    Class that represents a device.
    """

    def __init__(self, device_id, sensor_data, supervisor):
        """
        Constructor.

        @type device_id: Integer
        @param device_id: the unique id of this node; between 0 and N-1

        @type sensor_data: List of (Integer, Float)
        @param sensor_data: a list containing (location, data)
        as measured by this device

        @type supervisor: Supervisor
        @param supervisor: the testing infrastructure's control and
        validation component
        """
        self.device_id = device_id
        self.sensor_data = sensor_data
        self.supervisor = supervisor
        self.script_received = Event()
        self.scripts = []
        self.timepoint_done = Event()
        self.thread = DeviceThread(self)
        self.thread.start()

        self.devices_list = []
        self.barrier = None
        self.location_locks = {}

    def __str__(self):
        """
        Pretty prints this device.

        @rtype: String
        @return: a string containing the id of this device
        """
        return "Device %d" % self.device_id




    def setup_devices(self, devices):
        """
        Setup the devices before simulation begins.

        @type devices: List of Device
        @param devices: list containing all devices
        """
        # we don't need no stinkin' setup
        self.devices_list = devices

        if self.device_id == 0:
            self.barrier = Barrier(len(devices))

            for device in devices:
                device.barrier = self.barrier
                device.location_locks = self.location_locks


    def assign_script(self, script, location):
        """
        Provide a script for the device to execute.

        @type script: Script
        @param script: the script to execute from now on at each timepoint;
        None if the current timepoint has ended

        @type location: Integer
        @param location: the location for which the script is interested in
        """
        if script is not None:
            self.scripts.append((script, location))
        else:
            self.timepoint_done.set()
            self.script_received.set()

    def get_data(self, location):
        """
        Returns the pollution value this device has for the given location.

        @type location: Integer
        @param location: a location for which obtain the data

        @rtype: Float
        @return: the pollution value
        """
        return self.sensor_data[location] if location in self.sensor_data \
        else None

    def set_data(self, location, data):
        """
        Sets the pollution value stored by this device for the given location.

        @type location: Integer
        @param location: a location for which to set the data

        @type data: Float
        @param data: the pollution value
        """
        if location in self.sensor_data:
            self.sensor_data[location] = data

    def shutdown(self):
        """
        Instructs the device to shutdown (terminate all threads). This method
        is invoked by the tester. This method must block until all the threads
        started by this device terminate.
        """
        self.thread.join()

class DeviceThread(Thread):
    """
    Class that implements the device's worker thread.
    """

    def __init__(self, device):
        """
        Constructor.

        @type device: Device
        @param device: the device which owns this thread
        """
        Thread.__init__(self, name="Device Thread %d" % device.device_id)
        self.device = device

    def run(self):
        # hope there is only one timepoint, as multiple iterations of the
        # loop are not supported

        while True:
            # get the current neighbourhood
            neighbours = self.device.supervisor.get_neighbours()

            if neighbours is None:
                break

            self.device.script_received.wait()
            self.device.script_received.clear()


            # run scripts received until now
            for (script, location) in self.device.scripts:

                # creates a lock for a new location
                if not location in self.device.location_locks:
                    self.device.location_locks[location] = Lock()

                # only one script can run in a given location
                self.device.location_locks[location].acquire()

                script_data = []
                # collect data from current neighbours
                for device in neighbours:
                    data = device.get_data(location)
                    if data is not None:
                        script_data.append(data)
                # add our data, if any
                data = self.device.get_data(location)
                if data is not None:
                    script_data.append(data)

                if script_data != []:
                    # run script on data
                    result = script.run(script_data)

                    # update data of neighbours, hope no one is updating
                    # at the same time
                    for device in neighbours:
                        device.set_data(location, result)

                    # update our data, hope no one is updating at the same time
                    self.device.set_data(location, result)

                # finished running the script-> the lock can be released
                self.device.location_locks[location].release()

            # waiting for all the devices to finish before moving
            # to the next timepoint
            for device in self.device.devices_list:
                device.barrier.wait()

            # waiting for the "next-timepoint" signal
            self.device.timepoint_done.wait()

            # reseting and preparing for a new one
            self.device.timepoint_done.clear()
