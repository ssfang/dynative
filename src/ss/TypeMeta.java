package ss;

import java.lang.reflect.Field;

public class TypeMeta {
	byte sig;
	byte type;
	byte dimensions;
	byte element;
	int size;
	long meta;
	Field field;

	TypeMeta() {
	}

	TypeMeta(char sig, int t, int s, Field f) {
		this.sig = (byte) sig;
		type = (byte) t;
		size = s;
		field = f;
	}

	public static TypeMeta newMeta(Class<?> cls) {
		TypeMeta meta = new TypeMeta();
		MappedType mappedType = MappedType.of(cls);
		if (cls.isPrimitive()) {
			;
		} else {
			Class<?> componentType = cls.getComponentType();
			if (null == componentType) {
				long structMetaAddr = getStructMeta(cls, null);
				meta.meta = structMetaAddr;
			} else {
				Class<?> elementType;
				int ndims = 0;
				do {
					ndims++;
					elementType = componentType;
					componentType = componentType.getComponentType();
				} while (null != componentType);
				long structMetaAddr = getStructMeta(cls, null);
				meta.meta = structMetaAddr;
				// TODO size
				meta.size = 0; // cls.getAnnotation(ANativeType.class).size();
				if (ndims > Byte.MAX_VALUE) {
					throw new IllegalArgumentException(ndims + " dimensions is not allowed");
				}
				meta.dimensions = (byte) ndims;
				meta.element = (byte) MappedType.of(elementType).getSig().charAt(0);
			}
		}
		meta.sig = (byte) mappedType.getSig().charAt(0);
		meta.type = (byte) mappedType.ntype.typeid();
		meta.size = mappedType.ntype.size();
		return meta;
	}

	private static long getStructMeta(Class<?> cls, Object object) {
		return 0;
	}
}
