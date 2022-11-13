

    

class PID():
    def __init__(self, k_p = 2, k_i = 10, k_d = 20, dt):
        self.k_p = k_p
        self.k_i = k_i
        self.k_d = k_d
        self.integral_total_error = 0
        self.dt = dt
        self.previous_error = 0
        self.current_error = 0

    def update(self, current_angle):
        self.previous_error = self.current_error
        self.current_error = 0 - current_angle

        control_effort = self.k_p * self.current_error

        integral_total_error +=  self.current_error * self.dt
        control_effort += self.k_i * integral_total_error
        
        #we redo the derivative error everytime, we might want to keep some of the old value
        derivative_error =  ( self.current_error - self.previous_error ) / self.dt
        control_effort += self.k_d * derivative_error

        return control_effort
        
        
        
def pid(angle):
    pwm = 0
    return pwm

class Robot():
    def __init__(self, x, yaw, max_rpm, dt):
        self.x = x
        self.yaw = yaw
        self.max_rpm = max_rpm
    
    #returns the angle the robot will be given the control and the current angle (calculated from the baseline 0 which is upstraight)
    def motion_model(self, pwm):
        self.x = x
        self.yaw = yaw
        return x, yaw
    
    def pwm_to_rpm(pwm):
        rpm = pwm*self.max_rpm
        
    
        
if __name__ == "__main__":
    dt = 0.1
    init_x = 0
    init_yaw = 30
    max_rpm = 200
    rob = Robot(init_x, init_yaw, max_rpm, dt)
    pid_control = PID(dt=dt)
    for i in range(100):
        next_pwm = pid_control.update(rob.yaw)
        rob.motion_model(next_pwm)
        print(f"Robot current x: {rob.x} current yaw: {rob.yaw}")
        
    
    