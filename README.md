# CPU-Path-Tracer

A simple path tracer based on the ray tracing series from Two Minute Papers.
Parallelization is done using OpenMP, so make sure it's enabled.

Some visuals ! Both images are rendered with 4 threads & a resolution of 800x800.

1.000 Samples per pixel, 15 minutes rendering
![alt text](https://raw.githubusercontent.com/GoGreenOrDieTryin/CPU-Path-Tracer/master/ray_1000spp_15min.png)

10.000 Samples per pixel, 2 hours rendering (Same scene, I only slightly adjusted the left wall's color)
![alt text](https://raw.githubusercontent.com/GoGreenOrDieTryin/CPU-Path-Tracer/master/ray_10000spp_2h.png)

Note: The implementation is **basic**, no further optimizations have been made (yet).
      Variance could be significantly reduced e.g. by implementing Next Event Estimation.
