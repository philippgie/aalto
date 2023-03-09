struct Result
{
    float avg[3];
};

/*
This is the function you need to implement. Quick reference:
- x coordinates: 0 <= x < nx
- y coordinates: 0 <= y < ny
- horizontal position: 0 <= x0 < x1 <= nx
- vertical position: 0 <= y0 < y1 <= ny
- color components: 0 <= c < 3
- input: data[c + 3 * x + 3 * nx * y]
- output: avg[c]
*/
Result calculate(int ny, int nx, const float *data, int y0, int x0, int y1, int x1)
{
    Result result{{0.0f, 0.0f, 0.0f}};

    double avg[3] = {0};

    if ((ny < y1) || (nx < x1))
    {
        return result;
    }

    for (int x = x0; x < x1; x++)
    {
        for (int y = y0; y < y1; y++)
        {
            for (unsigned int c = 0; c < 3; c++)
            {
                avg[c] += data[c + 3 * x + 3 * nx * y];
            }
        }
    }

    for (unsigned int c = 0; c < 3; c++)
    {
        result.avg[c] = avg[c]/((x1 - x0) * (y1 - y0));
    }

    return result;
}
