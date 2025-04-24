# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText:  Copyright (C)  2025  Steffen A. Mork
#               All rights reserved */
#
# Blizzard III Raytracing library.

QT         -= gui core
TEMPLATE    = lib

include(../common.pri)

target.path = $$BLZ3_LIB

TARGET      = b3Raytrace
INSTALLS   += target

message("*** Blizzard III central raytracing library $$VERSION ***")

HEADERS += \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Activation.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Animation.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3BBox.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Base.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3BaseTrans.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Bump.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3BumpSampler.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3CameraProjection.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Condition.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Factory.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Light.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Material.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3MaterialSampler.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3PickInfo.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3PrepareInfo.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3RaytraceExif.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3RenderLight.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3RenderView.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Scaling.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Scene.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Shade.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Shape.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3ShapeRenderContext.h \
	$$BLZ3_INCLUDE/blz3/raytrace/b3Special.h \
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
	b3RaytraceExif.cc \
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
