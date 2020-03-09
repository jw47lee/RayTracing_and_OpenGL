-- A simple scene with some miscellaneous geometry.

--light green
mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
--gray
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
--orange
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)
--purple
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25)

scene_root = gr.node('root')

--
s1 = gr.nh_sphere('s1', {0, 0, -800}, 500)
scene_root:add_child(s1)
s1:set_material(mat1)

--moon
s2 = gr.nh_sphere('s2', {30, 30, 100}, 50)
scene_root:add_child(s2)
s2:set_material(mat2)

b1 = gr.nh_box('b1', {-60, 20, 30},30)
scene_root:add_child(b1)
b1:set_material(mat4)


red_light = gr.light({300.0, 150.0, 400.0}, {0.9, 0.0, 0.0}, {1, 0, 0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

--red_light = gr.light({0.0, 100.0, 300.0},{0.9, 0.0, 0.0},{1,0,0})
green_light = gr.light({-250.0, 150.0, 400.0}, {0.0, 0.9, 0.0}, {1,0,0})
blue_light = gr.light({0.0, -180.0, 400.0}, {0.0, 0.0,0.9}, {1,0,0})


gr.render(scene_root, 'sample.png', 500, 500,
	  {0, 0, 800}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {red_light, blue_light, green_light})

