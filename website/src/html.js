/* eslint-disable react/prop-types */
import React from 'react'
import { withPrefix } from 'gatsby'

const HTML = ({ headComponents, body, postBodyComponents }) => {
  return (
    <html xmlns="http://www.w3.org/1999/xhtml" dir="ltr" lang="en-US">
      <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="author" content="Anselme Chorein" />
        <meta name="copyright" content="Anselme Chorein" />
        <meta name="description" content="SilentEye is a cross-platform application design for an easy use of steganography, in this case hiding messages into pictures and sounds. It provides a pretty nice interface and an easy integration of new steganography algorithm and cryptography process by using a plug-ins system." />
        <meta name="keywords" content="steganography, cryptography, graphics, bmp, jpg, jpeg, image, picture, hide, encode, decode, windows, linux, mac, qt, qca, free, opensource" />
        <link rel="icon" href={withPrefix('/images/favicon.gif')} type="image/gif" />
        
        <title>SilentEye - Steganography is yours</title>
        
        <link rel="stylesheet" href={withPrefix('/css/style.css')} type="text/css" media="screen" />
        
        <link rel="stylesheet" href={withPrefix('/css/slider/slider.css')} type="text/css" media="screen" />
        <script type="text/javascript" src={withPrefix('/js/jquery-1.4.2.min.js')}></script>
        <script type="text/javascript" src={withPrefix('/js/script.js')}></script>
        {headComponents}
      </head>
      <body>
        {/* eslint-disable-next-line react/no-danger */}
        <div id="___gatsby" dangerouslySetInnerHTML={{ __html: body }} />
        {postBodyComponents}
      </body>
    </html>
  )
}

export default HTML
