import React from "react"
import Layout from "../components/Layout"

export default function Home() {
  return (
    <Layout>
<h3 style={{marginTop: 0}}>Changes since the last release</h3>

<h5 style={{marginBottom: 10}}>Version 0.4.2 - Changes since 0.4.1</h5>
<ul>
   <li><b>FEACTURES:</b></li>
   <ul>
     <li>configuration, log, lib and share paths are configurable</li>
     <li>"make install" works fine for linux distribution</li>
   </ul>
   <li><b>BUG FIXES:</b></li>
   <ul>
     <li>Installer now create symbolic link to right directory</li>
   </ul>
</ul>

<h5 style={{marginBottom: 10}}>Version 0.4.1b (only Mac OSX) - Changes since 0.4.1:</h5>
<ul>
	<li>Encryption (AES 128/256) didn't work on Mac OS X. Package (installer) had been updated to fix libraries dependences.</li>
</ul>

<h5 style={{marginBottom: 10}}>Version 0.4.1 - Changes since 0.4.0:</h5>
<ul>
	<li><b>FEACTURES:</b></li>
	<ul>
		<li>Encode message into JPEG file (SEFormatJPEG plug-in)</li>
	</ul>
	<li><b>ENHANCEMENTS:</b></li>
	<ul>
		<li>Enable manual edit for encoding destination</li>
		<li>Generate AES128 and AES256 key from MD5</li>
	</ul>
	<li><b>BUG FIXES:</b></li>
	<ul>
		<li>"Open file" function doesn't handle ".tif"</li>
	</ul>
	<li><b>INTERNAL:</b></li>
	<ul>
		<li>Update application and plu-ins for static build => needed by standalone installer</li>
	</ul>
</ul>

<h5 style={{marginBottom: 10}}>Version 0.4.0 - Changes since 0.3.1:</h5>
<ul>
	<li><b>ENHANCEMENTS:</b></li>
	<ul>
		<li>Audio file supported on main application(can open WAVE/PCM file)</li>
		<li>"Check for new version" feacture added</li>
		<li>New SEFormatWAV plug-in added (encode informations into wav files)</li>
		<li>New installer gives possibility to associate file extensions to silenteye (Windows only)</li>
		<li>Add tooltip showing absolute file path for current media</li>
		<li>Copy action works for images</li>
		<li>Add format selection on Property dialog (display capacity)</li>
		<li>SECryptoAES256 integration into application distribution package</li>
	</ul>
	<li><b>BUG FIXES:</b></li>
	<ul>
		<li>GUI update for more compatibility on KDE</li>
		<li>SEFormatBMP: change label to "bit(s) per pixel per color"</li>
		<li>Open two files with same shortname and diferent localisation</li>
		<li>Now when decoding process succeed, an other try on decode button works.</li>
	</ul>
	<li><b>INTERNAL:</b></li>
	<ul>
		<li>SilentEyeFramework architecture is now ready for video plug-in</li>
	</ul>
	<li><b>DEVELOPER RELATED:</b></li>
	<ul>
		<li>CMake now search QCA2 on Qt directories</li>
		<li>Move to Qt 4.7.0 or later (using last QtMultimedia)</li>
		<li>Move to CMake 2.8.3 (update for QtMultimedia)</li>
	</ul>
</ul>

<h5 style={{marginBottom: 10}}>Version 0.3.1 - Changes since 0.3.0</h5>
<ul>
	<li><b>BUG FIXES:</b></li>
	<ul>
		<li>"Can't use equidistribution mode for most image and message." : SEFormatBMP plug-in updated to version 1.0.1</li>
		<li>GUI update for more compatibility when using Gnome.</li>
	</ul>
</ul>

<h5 style={{marginBottom: 10}}>Version 0.3.0 - Changes since 0.2.0</h5>
<ul>
	<li><b>NEW FEACTURES:</b></li>
	<ul>
		<li>Data compression</li>
		<li>Capacity to hide a file</li>
		<li>Improvement of BMP plug-in</li>
		<li>Add module state into preferences dialog</li>
	</ul>
	<li><b>REPORTED FEACTURES:</b></li>
	<ul>
		<li>Use of context menu in Windows explorer reported to next version</li>
	</ul>
</ul>

<h5 style={{marginBottom: 10}}>Version 0.2.0 - Changes since 0.1.1</h5>
<ul>
	<li><b>NEW FEACTURES:</b></li>
	<ul>
		<li>Plug-ins system</li>
		<li>Encryption support (QCA2)</li>
		<li>Adding logger</li>
	</ul>
</ul>

<h5 style={{marginBottom: 10}}>Version 0.1.1 - Changes since 0.1.0</h5>
<ul>
	<li><b>NEW FEACTURES:</b></li>
	<ul>
		<li>Keep image ratio on window resize event</li>
	</ul>
	<li><b>BUGS:</b></li>
	<ul>
		<li>many bugs fixed</li>
	</ul>
</ul>
</Layout>
  )
}