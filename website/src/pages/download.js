import React from "react"
import Layout from "../components/Layout"
import { Link, withPrefix } from "gatsby"

export default function Home() {
  return (
    <Layout>
<h3 style={{marginTop: 0}}>Application</h3>
<table class="table center" style={{margin: 10}}>
  <tr>
	<td>

	  <img src={withPrefix('/images/icon/windows.png')} alt="Windows" class="image" style={{marginBottom: 7}} /><br/>
	  Version 0.4.1<br/>
	  <a href="http://sourceforge.net/projects/silenteye/files/Application/0.4/silenteye-0.4.1-win32.exe/download">Download <b>Windows 32bit</b> installer</a>
	</td>
	<td>
	  <a href="#"><img src={withPrefix('/images/icon/macosx.png')} alt="Mac OSX" class="image" style={{marginBottom: 7}} /></a><br/>
	  Version 0.4.1 for <b>Mac OSX</b><br/>
	  <a href="http://sourceforge.net/projects/silenteye/files/Application/0.4/silenteye-0.4.1b-snowleopard.dmg/download">Download <b>Snow Leopard</b> package</a>
	</td>
	<td>
	  <img src={withPrefix('/images/icon/linux.png')} alt="Linux" class="image" style={{marginBottom: 7}} /><br/>
	  Version 0.4.2 for <b>Linux</b><br/>
	  <a href="http://sourceforge.net/projects/silenteye/files/Application/0.4/silenteye-0.4.2.beta-ubuntu-12.04.run/download">Download <b>Ubuntu 12.04</b> package</a>
	  
	</td>
  </tr>
</table><br/>
<p>
	<b>Other :</b>
	<ul>
		<li>See the <Link to="/changes/?i1s3">change log</Link> to find out what has changed<br/></li>
		<li>Older releases are available from <a href="http://sourceforge.net/projects/silenteye/files/">sourceforge</a></li>
	</ul>
</p>
<br/>
<h3>Source</h3>
<p>
  <img src={withPrefix('/images/icon/source.png')} alt="plug-ins" class="image" style={{margin: '0 10px 0 0', float: 'left'}}/>
  <div style={{padding: 10}}>

	SilentEye is a <a href="http://qt.nokia.com/products/">Qt 4</a> application writing in C++ and using <a href="http://www.cmake.org/">CMake</a> as compilation tool. 
	You can download application and plug-ins source by clicking on the following link : <a href="http://sourceforge.net/projects/silenteye/files/Source/silenteye-0.4.1-src.zip/download">silenteye-0.4.1-src.zip</a><br/>
	Current developpment version can also be clone from github : <a href="https://github.com/achorein/silenteye/">https://github.com/achorein/silenteye/</a>
  </div>
</p>
</Layout>
  )
}
