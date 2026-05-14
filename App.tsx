// src/useESP32Socket.ts
import Joystick from 'rc-joystick'
import { useEffect, useMemo, useRef, useState } from 'react'

const ESP32_IP = '192.168.2.179' // ← replace with your ESP32's actual IP

const MINIMUM_DISTANCE = 40
const directions = [
	'moveRight',
	'moveFR',
	'moveForward',
	'moveFL',
	'moveLeft',
	'moveRL',
	'moveBackward',
	'moveRR',
	'moveRight',
]

// 'moveCW',
// 	'moveCCW',
// 	'Stop',
type Direction = (typeof directions)[number]

interface UseESP32SocketReturn<T> {
	data: T | null
	connected: boolean
	send: (msg: string | object) => void
}

function getAction(angle: number, distance: number): Direction | 'Stop' {
	if (distance < MINIMUM_DISTANCE) return 'Stop'
	// const count = Math.floor(angle / 22.5)
	// console.log(directions[Math.round(angle / 45)])
	return directions[Math.round(angle / 45)] ?? 'Stop'
}

export function App() {
	const ws = useRef<WebSocket | null>(null)
	const [data, setData] = useState<any>(null)
	const [connected, setConnected] = useState<boolean>(false)

	useEffect(() => {
		let timeoutId: number

		function connect() {
			ws.current = new WebSocket(`ws://${ESP32_IP}/ws`)

			ws.current.onopen = () => {
				console.log('WebSocket connected')
				setConnected(true)
				ws.current?.send('ping') // optional handshake
			}

			ws.current.onmessage = (event: MessageEvent) => {
				try {
					// Attempt to parse JSON
					setData(JSON.parse(event.data))
				} catch {
					// Fallback to raw data if not JSON
					setData(event.data)
				}
			}

			ws.current.onclose = () => {
				console.log('WebSocket closed — retrying in 2 s')
				setConnected(false)
				timeoutId = window.setTimeout(connect, 2000) // auto-reconnect
			}

			ws.current.onerror = (err) => {
				console.error('WebSocket error:', err)
			}
		}

		connect()

		// Cleanup on unmount
		return () => {
			ws.current?.close()
			window.clearTimeout(timeoutId)
		}
	}, [])

	const send = (msg: string | object) => {
		if (ws.current?.readyState === WebSocket.OPEN) {
			const payload = typeof msg === 'string' ? msg : JSON.stringify(msg)
			ws.current.send(payload)
		}
	}

	const [angle, setAngle] = useState(0)
	const [distance, setDistance] = useState(0)

	const action = useMemo(() => getAction(angle, distance), [angle, distance])

	useEffect(() => {
		send(action)
	}, [action])

	return (
		<>
			<div className="absolute top-0 left-0 w-screen h-screen ">
				<div className="flex flex-col gap-4">
					<div className="font-mono px-4 py-1 rounded-md border mx-auto w-fit bg-neutral-50">
						<span className="font-sans mr-2 text-sm">Angle:</span>
						{getAction(angle, distance)}
					</div>
					<div className="font-mono px-4 py-1 rounded-md border mx-auto w-fit bg-neutral-50">
						<span className="font-sans mr-2 text-sm">Active:</span>
						{distance > MINIMUM_DISTANCE ? 'Yes' : 'No'}
					</div>
				</div>
				{/* <Button
					onClick={() => {
						send('moveForward')
					}}
				>
					move forward
				</Button>
				<Button
					onClick={() => {
						send('Stop')
					}}
				>
					Stop
				</Button> */}
				<Joystick
					onAngleChange={(angle) => {
						setAngle(angle ? Math.round(angle) : 0)
					}}
					onDistanceChange={(distance) => {
						setDistance(distance ? Math.round(distance) : 0)
					}}
					className="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2"
				/>
			</div>
		</>
	)

	// return { data, connected, send }
}
