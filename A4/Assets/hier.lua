-- test for hierarchical ray-tracers.
-- Thomas Pflaum 1996

gold = gr.material(	 {0.9, 0.8, 0.4}, 	{0.8, 0.8, 0.4}, 	25, 1.0)
grass = gr.material( {0.1, 0.7, 0.1}, 	{0.0, 0.0, 0.0}, 	0,  1.0)
blue = gr.material(  {0.7, 0.6, 1}, 	{0.5, 0.4, 0.8}, 	25, 1.0)
glass = gr.material( {0, 0, 0}, 		{1, 1, 1}, 			25, 1.5);

scene = gr.node('scene')
scene:rotate('X', 23)
scene:translate(6, -2, -15)

-- s0 = gr.nh_sphere('s0', {0, 0, -10}, 2)
-- scene:add_child(s0)
-- s0:set_material(blue)
-- s0:translate(-1, 0, -10)
-- s0:scale(0.5, 0.5, 0.5)

-- s1 = gr.nh_sphere('s1', {5, 0, -5}, 2)
-- s0:add_child(s1)
-- s1:set_material(gold)
-- s0:translate(1, 0, -10)

-- the arc
arc = gr.node('arc')
scene:add_child(arc)
arc:translate(0,0,-10)
arc:rotate('Y', 60)

-- s = gr.sphere('s')
-- arc:add_child(s)
-- s:set_material(gold)
-- s:scale(1, 1, 2)
-- s:translate(0, 0, -10)

p1 = gr.cube('p1')
arc:add_child(p1)
p1:set_material(gold)
p1:scale(0.8, 4, 0.8)
p1:translate(-2.4, 0, -0.4)

p2 = gr.cube('p2')
arc:add_child(p2)
p2:set_material(gold)
p2:scale(0.8, 4, 0.8)
p2:translate(1.6, 0, -0.4)

s = gr.sphere('s')
arc:add_child(s)
s:set_material(gold)
s:scale(4, 0.6, 0.6)
s:translate(0, 4, 0)

-- the floor
-- plane = gr.mesh( 'plane', 'Assets/plane.obj' )
plane = gr.plane('plane')
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)

-- sphere
poly = gr.mesh( 'poly', 'Assets/dodeca.obj' )
scene:add_child(poly)
poly:translate(-2, 1.618034, 0)
poly:set_material(blue)


-- reflective sphere
reflectiveSphere = gr.sphere("reflective")
scene:add_child(reflectiveSphere)
reflectiveSphere:set_material(glass)
reflectiveSphere:scale(1, 1, 1)
reflectiveSphere:translate(-7, 2, 0)

-- cylinder
-- cylinder = gr.cylinder('cylinder')
-- scene:add_child(cylinder)
-- cylinder:set_material(blue)
-- cylinder:scale(0.5, 2, 0.5)
-- cylinder:translate(-9, 2, 3)
-- cylinder:rotate('X', -30)
-- cylinder:rotate('Z', 90)

-- cone = gr.cone('cone')
-- scene:add_child(cone)
-- cone:set_material(blue)
-- cone:translate(-5, 0, 3)
-- cone:scale(1, 2, 1)

-- The lights
l1 = gr.light({200,200,400}, {0.8, 0.8, 0.8}, {1, 0, 0})
l2 = gr.light({0, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0})

gr.render(scene, 'hier.png', 1024, 1024,
	  {0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {l1, l2})
