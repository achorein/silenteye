import React from "react"
import Layout from "../components/Layout"
import { Link } from "gatsby"

export default function Home() {
  return (
    <Layout>
<h3 style={{marginTop: 0}}>How can i write my own plug-in?</h3>

<p>
  First you have to <Link to="/download/?i2">download source code</Link> 
  of the project and make sure you are able to <Link to="/devenv/?i3s1">compile it</Link>.
</p>

<h4>1. Write an image format plug-in</h4>

<p>
  You just need to implement the ImageModuleInterface class which have to provide a specialized implentation of Image class. You can find an example in <i>src/modules/seformatbmp/</i> directory.<br/>
</p>
<h5>1.1 Implementation of ImageModuleInterface</h5>
<p>
  Let's try to add a plugin for support of PNG. You first have to extend the module interface and then specify to Qt that you are about to write a plugin:
  <p class="code">
	<span class="codename">myformatpng.h</span>
	#include &lt;imagemoduleinterface.h&gt;<br/>
	<br/>
	using namespace SilentEyeFramework;<br/>
	<br/>
	namespace MyFormatPNG {'{'}<br/>
	<br/>
	class MyFormatPNG <b>: public ImageModuleInterface</b><br/>
	{'{'}<br/>
	<b>Q_OBJECT;<br/>
	Q_INTERFACES(SilentEyeFramework::ImageModuleInterface);</b><br/>
	<br/>
	public:<br/>
	&nbsp; MyFormatPNG();<br/>
	&nbsp; QWidget* encodeWidget();<br/>
	&nbsp; QWidget* decodeWidget();<br/>
	&nbsp; bool isEncodeWidgetReady() const;<br/>
	&nbsp; bool isDecodeWidgetReady() const;<br/>
	<br/>
	&nbsp; QPointer&lt;Image&gt; encodeImage(QPointer&lt;Image&gt;,bool=false);<br/>
	&nbsp; QPointer&lt;Image&gt; decodeImage(QPointer&lt;Image&gt;,bool=false);<br/>
	<br/>
	&nbsp; QString typeSupported() const;<br/>
	<br/>
	&nbsp; QString name() const;<br/>
	&nbsp; QString version() const;<br/>
	&nbsp; QString status();<br/>
  </p>
  Implementation must reference itself to the Qt plugin system.
  <p class="code">
	<span class="codename">myformatpng.cpp</span>
	#include "myformatpng.h"<br/>
	<br/>
	...<br/>
	<br/>
	<b>Q_EXPORT_PLUGIN2(myformatpng, MyFormatPNG::MyFormatPNG)</b>
  </p>
</p>
<h5>1.2 Implementation of Image</h5>
<p>
  <p class="code">
	<span class="codename">myimagepng.cpp</span>
	<b>#include &lt;image.h&gt;</b><br/>
	<br/>
	using namespace SilentEyeFramework;<br/>
	<br/>
	namespace MyFormatPNG {'{'} <br/>
	<br/>
	class MyImagePNG : <b>public Image</b><br/>
	{'{'}<br/>
	...
  </p>
  In this class, 3 methods are important and must be override:
  <ul>
	<li>quint32 capacity() const; : must return the max number of byte we can hide into the current image.</li>
	<li>bool loadData(); : read current media in order to extract hidden information. Result must be setted into m_data</li>
	<li>bool saveToDir(QString) : hide message into the current media and save current output into given destination folder</li>
  </ul>
  Information to encode is provide as an EncodedData object. This is an easy to use objet for a bit per bit read. See API documentation for more information.
</p>
<h5>1.3 Provide a configuration widget</h5>
<p>
  You can provide a configuration widget for the SilentEye GUI. This QWidget will be retreive by "encodeWidget()" and "decodeWidget()" methods of your ModuleFormatInterface implementation.
</p>

<h4>2. Write a cryptography plug-in</h4>

<p>
  <blockquote>
	Coming soon !
  </blockquote>
</p>
    </Layout>
	)
  }