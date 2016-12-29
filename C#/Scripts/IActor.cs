using UnityEngine;

namespace AISandbox {
    public interface IActor {
        void SetInput( float x_axis, float y_axis, float x2_axis = 0.0f, float y2_axis = 0.0f );
        float MaxSpeed { get; }
        Vector3 Velocity { get; }
    }
}