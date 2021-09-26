# Pose of the object
h2sl/Transform3DMessage pose
# Bounding box of the object
h2sl/BoundingBoxMessage bbox
# Linear velocity of the object
h2sl/Vector3DMessage v
# Angular velocity of the object
h2sl/Vector3DMessage w
# Timestamp of the ObjectState in nanoseconds
float64 t

# The following fields indicate which of the above fields are non-empty
# Zero = False, Non-zero = True
bool pose_exists
bool bbox_exists
bool v_exists
bool w_exists
bool t_exists
