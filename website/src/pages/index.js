import React from "react"
import Layout from "../components/Layout"
import { Link, withPrefix } from "gatsby"

export default function Home() {
  return (
    <Layout>
		<h3 style={{marginTop: 0}}>What is SilentEye?</h3>
		<p>
			<i>SilentEye</i> is a cross-platform application design for an easy use of steganography, in this case hiding messages into pictures or sounds.
			It provides a pretty nice interface and an easy integration of new steganography algorithm and cryptography process by using a plug-ins system.<br/>
			<i>SilentEye</i> is free to use (under GNU GPL v3).
		</p>
		
		<h4>Main Features</h4>
		<p>
			<ul>
				<li>Hide information into images and sounds (LSB)</li>
				<ul>
					<li><b>JPEG</b></li>
					<li>BMP</li>
					<li>WAVE</li>
				</ul>
				<li>Encrypte data</li>
				<ul>
					<li>AES128</li>
					<li><b>AES256</b></li>
				</ul>
				<li>Capacity to hide text or file</li>
				<li>zlib compression of message</li>
				<li>Drag & Drop</li>
			</ul>
		</p>
		<br/>
		<p>
			<b>Compatible with:</b>
			<table class="table center" style={{margin: 10}}>
				<tr>
					<td>
						<Link to="/download/?i2"><img src={withPrefix('/images/icon/windows.png')} alt="Windows" class="image" style={{marginBottom: 7}} /></Link><br/>
						<b>Windows</b>
					</td>
					<td>
						<Link to="/download/?i2"><img src={withPrefix('/images/icon/macosx.png')} alt="Mac OSX" class="image" style={{marginBottom: 7}} /></Link><br/>
						<b>Mac OS X</b>
					</td>
					<td>
						<Link to="/download/?i2"><img src={withPrefix('/images/icon/linux.png')} alt="Linux" class="image" style={{marginBottom: 7}} /></Link><br/>
						<b>Linux</b>
					</td>
				</tr>
			</table>
		</p>
		
		<h4>Architecture</h4>
		<p>
			Media format and encryption are supported by plug-ins :
			<ul style={{textAlign: 'left'}}>
				<li>Format Plug-in :
					<ul>
						<li>Image format plug-ins (ex: BMP, JPEG), which allow you to save informations into output files (ex: .jpeg).</li>
						<li>Audio format plug-ins (ex: WAVE), provided output for .wav files</li>
					</ul>
				</li>
				<li>Cryptography plug-ins:</li>
				<ul>
					<li>Allow application to encrypte data before hidding them (ex: AES 256)</li>
				</ul>
			</ul>
			Using this architecture provides easy integration of new steganography algorithm and cryptography process.
		</p>
		<br/>
    </Layout>
  )
}
