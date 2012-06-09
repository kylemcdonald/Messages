/*
 GLSL fragment shader implementing the one line synthesis pattern.
 http://countercomplex.blogspot.com/2011/10/algorithmic-symphonies-from-one-line-of.html
*/

#extension GL_EXT_gpu_shader4 : enable
const int width = 512; // assumes you're drawing at 512x512
void main() {
	int it = int(gl_FragCoord.y) * width + int(gl_FragCoord.x);
	float ft = float(it);
	float q = (it / (512 * 100)) % 2 > 0 ? .4 : .2;
	int left = int((sin(ft * .02 * clamp(sin(ft / .0001), q, 1.)) + sin(ft * .03)) * .5 * 127. + 128.);
	float ping = float(it % (512 * 64));
	int right = int(sin(ft * .01) +
		(512.)/ping * sin(ping * .001 * abs(cos(ping * .1)))
		* .5 * 127. + 128.);
	gl_FragColor = vec4(
		float(left % 256) / 256.,
		float(right % 256) / 256.,
		0,
		1.);
}
