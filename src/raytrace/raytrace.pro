# Blizzard III Base library

CONFIG   += c++14
TEMPLATE  = lib

BLZ3_HOME = ../..
BLZ3_INCLUDE = $$BLZ3_HOME/include
BLZ3_LIB     = $$BLZ3_HOME/lib

unix
{
	INCLUDEPATH += $$BLZ3_HOME/include_unix
}

INCLUDEPATH += $$BLZ3_INCLUDE
TARGET       = b3Raytrace

target.path = $$BLZ3_LIB

INSTALLS  += target

HEADERS += \
	../../include/blz3/raytrace/b3Activation.h \
	../../include/blz3/raytrace/b3Animation.h \
	../../include/blz3/raytrace/b3BBox.h \
	../../include/blz3/raytrace/b3Base.h \
	../../include/blz3/raytrace/b3BaseTrans.h \
	../../include/blz3/raytrace/b3Bump.h \
	../../include/blz3/raytrace/b3BumpSampler.h \
	../../include/blz3/raytrace/b3CameraProjection.h \
	../../include/blz3/raytrace/b3Condition.h \
	../../include/blz3/raytrace/b3Factory.h \
	../../include/blz3/raytrace/b3Light.h \
	../../include/blz3/raytrace/b3Material.h \
	../../include/blz3/raytrace/b3MaterialSampler.h \
	../../include/blz3/raytrace/b3PickInfo.h \
	../../include/blz3/raytrace/b3PrepareInfo.h \
	../../include/blz3/raytrace/b3Raytrace.h \
	../../include/blz3/raytrace/b3RenderLight.h \
	../../include/blz3/raytrace/b3RenderView.h \
	../../include/blz3/raytrace/b3Scaling.h \
	../../include/blz3/raytrace/b3Scene.h \
	../../include/blz3/raytrace/b3Shade.h \
	../../include/blz3/raytrace/b3Shape.h \
	../../include/blz3/raytrace/b3ShapeRenderContext.h \
	../../include/blz3/raytrace/b3Special.h \
	b3RaytraceInclude.h \
	b3ReadCOB.h \
	b3ReadTGF.h

SOURCES += \
	b3AnimElement.cc \
	b3Animation.cc \
	b3Area.cc \
	b3BBox.cc \
	b3BaseTrans.cc \
	b3Box.cc \
	b3Bump.cc \
	b3BumpSampler.cc \
	b3CSGBox.cc \
	b3CSGCone.cc \
	b3CSGCylinder.cc \
	b3CSGEllipsoid.cc \
	b3CSGShape.cc \
	b3CSGSphere.cc \
	b3CSGTorus.cc \
	b3CameraProjection.cc \
	b3Condition.cc \
	b3Cone.cc \
	b3Cylinder.cc \
	b3Disk.cc \
	b3Ellipsoid.cc \
	b3Factory.cc \
	b3Init.cc \
	b3Intersect.cc \
	b3Light.cc \
	b3Material.cc \
	b3MaterialSampler.cc \
	b3Normal.cc \
	b3NormalDeriv.cc \
	b3PickInfo.cc \
	b3PrepareInfo.cc \
	b3RayRow.cc \
	b3Raytrace.cc \
	b3RaytraceInclude.cc \
	b3ReadCOB.cc \
	b3ReadTGF.cc \
	b3RenderLight.cc \
	b3RenderShape.cc \
	b3RenderView.cc \
	b3Scaling.cc \
	b3Scene.cc \
	b3SceneBBoxTraversal.cc \
	b3Shade.cc \
	b3ShadeMork.cc \
	b3ShadeMork2.cc \
	b3ShadePhong.cc \
	b3Shape.cc \
	b3Special.cc \
	b3Sphere.cc \
	b3SplineArea.cc \
	b3SplineCylinder.cc \
	b3SplineRing.cc \
	b3SplineRot.cc \
	b3SplineShape.cc \
	b3Torus.cc \
	b3TriangleShape.cc \
	b3Triangles.cc
