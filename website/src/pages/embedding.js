import React from "react"
import Layout from "../components/Layout"
import { withPrefix } from "gatsby"

export default function Home() {
  return (
    <Layout>
<h3 style={{marginTop: 0}}>How does it work?</h3>

<h4>How does SilentEye embed data in BMP format ?</h4>
<p>
	SEFormatBMP plug-in use LSB (<a href="http://en.wikipedia.org/wiki/Least_significant_bit">Least Significant Bit</a>) process
	in order to hide data into <a href="http://en.wikipedia.org/wiki/RGB_color_model#Digital_representations">RGB</a> representation of a pixel. 
	Has <a href="http://en.wikipedia.org/wiki/Bitmap">Bitmap</a> is not a compressed format there is no need to worry about losing information.
</p>
<br/>
<p>
	<b>Diagram :</b><br/>
	<a href={withPrefix('/images/embedding/seformatbmp-embedding_full.png')}><img src={withPrefix('/images/embedding/seformatbmp-embedding.png')}/></a>
</p>
<br/>
<p>
	<b>See also :</b><br/>
	<ul>
		<li><a href="http://en.wikipedia.org/wiki/Bitmap">BMP Format</a></li>
		<li><a href="http://en.wikipedia.org/wiki/RGB_color_model#Digital_representations">RGB color model</a></li>
		<li><a href="http://en.wikipedia.org/wiki/Least_significant_bit">Least Significant Bit (LSB)</a></li>
	</ul>
</p>
<br/>
<h4>What does "distribution" means for SilentEye ?</h4>
<b>Distibution mode</b><br/>
<img src={withPrefix('/images/embedding/seformatbmp-distribution.png')}/>

<h4>What does "header" means for SilentEye ?</h4>

<p>
In order to determine how many bits have to be read from image, the embedded data size is compute and writen into media (32 bits are reserved for header). 
Header also contains data format (text, charset / file, filename).
</p>
<br/>
<b>Header position</b><br/>
<img src={withPrefix('/images/embedding/seformatbmp-header.png')}/>
</Layout>
  )
}