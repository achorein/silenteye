import React from "react"
import Layout from "../components/Layout"

export default function Home() {
  return (
    <Layout>
		<h3 style={{marginTop: 0}}>About SilentEye</h3>

		SilentEye is a <a href="COPYING.txt">GNU General Public License version 3.0 (GPLv3)</a> project, created in 2008 by Anselme CHOREIN. <br/>
		<br/>
		Project objectives are : 
		<ul>
			<li>provide a free and cross-platform application for steganography use</li>
			<li>provide a sexy interface for end user</li>
		</ul>
		<br/>
		SilentEye is a <a href="http://qt.nokia.com/products/">Qt 4</a> application written in C++.<br/>

		<h3>About author</h3>

		SilentEye project is developped by <a href="mailto:silenteye@chorein.fr">Anselme CHOREIN</a>, a french IT Engineer.

		<h3>About image</h3>

		Most of the image used for SilentEye have been created by the following author : 
		<ul>
			<li>Author: Afterglow (Cian Walsh)</li>
			<ul>
				<li>Homepage: 	<a href="http://www.afterglow.ie">afterglow.ie</a></li>
				<li>License: Free for non-commercial use</li>
				<li>ReadMe: <a href="licence/afterglow/readme.html">readme.html</a></li>
			</ul>
			<li>Author: Mattahan (Paul Davey)</li>
			<ul>
				<li>Homepage: 	<a href="http://mattahan.deviantart.com/">mattahan.deviantart.com</a></li>
				<li>License: <a href="http://creativecommons.org/licenses/by-nc-sa/3.0/">CC Attribution-Noncommercial-Share Alike 3.0</a></li>
			</ul>
			<li>Author: IconTexto (Bruno Maia)</li>
			<ul>
				<li>Homepage: 	<a href="http://www.icontexto.com">icontexto.com</a></li>
				<li>License: <a href="http://creativecommons.org/licenses/by-nc/3.0/">CC Attribution-Noncommercial 3.0</a></li>
				<li>Readme: <a href="licence/icontexto/copyright.jpg">copyright.jpg</a></li>
			</ul>
		</ul>
  	</Layout>
  )
}
