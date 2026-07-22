#pragma once

namespace Engine
{
    struct OutlinedRect;
    class LineGeometryBuilder;
    class IGeometrySink;

    class OutlinedRectGeometryBuilder
    {
    public:
        explicit OutlinedRectGeometryBuilder(LineGeometryBuilder& lineBuilder);

        void build(const OutlinedRect& orect, IGeometrySink& sink);

    private:
        LineGeometryBuilder& m_lineBuilder;
    };
}