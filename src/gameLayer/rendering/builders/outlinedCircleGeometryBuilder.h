#pragma once

namespace Engine
{
    struct OutlinedCircle;
    class LineGeometryBuilder;
    class IGeometrySink;

    class OutlinedCircleGeometryBuilder
    {
    public:
        explicit OutlinedCircleGeometryBuilder(LineGeometryBuilder& lineBuilder);

        void build(const OutlinedCircle& orect, IGeometrySink& sink);

    private:
        LineGeometryBuilder& m_lineBuilder;
    };
}