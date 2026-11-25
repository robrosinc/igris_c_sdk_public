float32 q            # rad
float32 dq           # rad/s
float32 tau_est      # Nm
uint32 status_bits   # same flags as MotorState (see its constants; not repeated here
                     # because the Python bindings expose constants in one flat namespace)
