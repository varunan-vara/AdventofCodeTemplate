#ifndef AOCUTILS_PLOT
#define AOCUTILS_PLOT

// Thin helpers over Matplot++, for the boilerplate of drawing one plot. Not part of AoCUtils.h:
// including this header makes aoc_add_challenges fetch and link Matplot++ for the challenge.
// Needs gnuplot at runtime (brew install gnuplot)
#include <matplot/matplot.h>

#include <string>
#include <vector>

namespace AoCUtils::plot
{
    // What every plot function can set. Leave a field empty to skip it
    struct Options {
        std::string title;
        std::string xlabel;
        std::string ylabel;
        // File to save to (ex. "out.png"). Shows the plot in a window when empty
        std::string saveTo;
    };

    // Applies the options to the current figure, then saves or shows it
    inline void finish(const Options &options) {
        if (!options.title.empty()) matplot::title(options.title);
        if (!options.xlabel.empty()) matplot::xlabel(options.xlabel);
        if (!options.ylabel.empty()) matplot::ylabel(options.ylabel);

        if (options.saveTo.empty()) {
            matplot::show();
        } else {
            matplot::save(options.saveTo);
        }
    }

    // Line through the points (x[i], y[i])
    inline void line(const std::vector<double> &x, const std::vector<double> &y, const Options &options = {}) {
        matplot::plot(x, y);
        finish(options);
    }

    // The points (x[i], y[i]), unconnected
    inline void scatter(const std::vector<double> &x, const std::vector<double> &y, const Options &options = {}) {
        matplot::scatter(x, y);
        finish(options);
    }

    // Colored cells for a grid, row 0 at the top (as InputFile::get2DArray reads it). Any numeric
    // or char type works: chars are colored by their character code
    template <typename T>
    void grid(const std::vector<std::vector<T>> &cells, const Options &options = {}) {
        std::vector<std::vector<double>> values;
        for (const std::vector<T> &row : cells) {
            values.emplace_back(row.begin(), row.end());
        }
        matplot::imagesc(values);
        matplot::gca()->y_axis().reverse(true);
        finish(options);
    }
} // namespace AoCUtils::plot

#endif /*AOCUTILS_PLOT*/
